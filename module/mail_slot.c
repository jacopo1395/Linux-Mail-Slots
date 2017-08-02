#define EXPORT_SYMTAB
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/sched.h> // serve per current->pid
#include <linux/version.h>  /* For LINUX_VERSION_CODE */

#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/spinlock.h>
#include <linux/semaphore.h>

#include <linux/pid.h>      /* For pid types */
#include <linux/tty.h>      /* For the tty declarations */
#include <linux/version.h>  /* For LINUX_VERSION_CODE */
#include "mail_struct.h"
#include "helper.h"
#include "config.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jacopo Carlini");


#define DEVICE_NAME "mail_slot"  /* Device file name in /dev/ - not mandatory  */
#define MOD_NAME "Mail Slot"



static int Major;            /* Major number assigned to broadcast device driver */



instances my_instances;
struct semaphore my_semaphores_read[MAX_INSTANCES];
struct semaphore my_semaphores_write[MAX_INSTANCES];

/* the actual driver */
static int mail_open(struct inode* inode, struct file* file) {
    dev_t info = inode->i_rdev;
    int minor = MINOR(info);
    printk(KERN_INFO "%s Open:  minor=%d , pid=%d\n", MOD_NAME, minor, current->pid);
    ERROR_HELPER(minor >= MAX_INSTANCES, "too many istances");


    return SUCCESS;
}


static int mail_release(struct inode* inode, struct file* file) {
    dev_t info = inode->i_rdev;
    int minor = MINOR(info);
    printk(KERN_INFO "%s Release:  minor=%d , pid=%d\n", MOD_NAME, minor, current->pid);


    return SUCCESS;
}


static ssize_t mail_write(struct file* filp, const char* buff, size_t len, loff_t* off) {
    int ret;
    int minor = iminor(filp->f_path.dentry->d_inode);
    mailbox* my_mailbox = &(my_instances.list[minor]);
    mail* my_message = (mail*)kmalloc(sizeof(mail), GFP_KERNEL);
    // mail* aux;
    printk(KERN_INFO "%s Write:  minor=%d , pid=%d\n", MOD_NAME, minor, current->pid);
    WARNING_HELPER(len >= max_msg_len, "message length too high");
    if (blocking_write) {
        down_interruptible(&(my_semaphores_write[minor]));
    }
    else {
        down_trylock(&(my_semaphores_write[minor]));
        spin_lock(&(my_mailbox->lock));
        if (atomic_read(&(my_mailbox->len)) >= max_msg) {
            spin_unlock(&(my_mailbox->lock));
            WARNING_HELPER( 1, "mailbox is full");
        }
        else spin_unlock(&(my_mailbox->lock));
    }


    my_message->next = NULL;
    my_message->txt = (char*)kmalloc(sizeof(char) * len, GFP_KERNEL);
    my_message->len = len;

    ret = copy_from_user(my_message->txt, buff, len);
    ERROR_HELPER(ret < 0, "copy_from_user failed");


    // print_mail(my_message);

    spin_lock(&(my_mailbox->lock));
    // aux = my_mailbox->head;
    if (atomic_read(&(my_mailbox->len)) == 0) {
        my_mailbox->head = my_message;
        my_mailbox->tail = my_message;
    }
    else{
        my_mailbox->tail->next = my_message;
        my_mailbox->tail = my_message;
    }
    atomic_inc(&(my_mailbox->len));
    print_mailbox(my_mailbox);
    printk("%s: write quasi fine\n", MOD_NAME);
    up(&(my_semaphores_read[minor]));
    spin_unlock(&(my_mailbox->lock));

    printk("%s: write end , minor=%d , pid=%d\n", MOD_NAME, minor, current->pid);

    return len;
}


static ssize_t mail_read(struct file* filp, char* buff, size_t len, loff_t* off) {
    int ret, l;
    int minor = iminor(filp->f_path.dentry->d_inode);


    mailbox* my_mailbox = &(my_instances.list[minor]);
    mail* my_message;
    mail* head;
    printk(KERN_INFO "%s Read:  minor=%d , pid=%d\n", MOD_NAME, minor, current->pid);

    DEBUG_INFO("qui 1");



    if (blocking_read) {
        down_interruptible(&(my_semaphores_read[minor]));
        spin_lock(&(my_mailbox->lock));
    }
    else{
        down_trylock(&(my_semaphores_read[minor]));
        spin_lock(&(my_mailbox->lock));
        if (atomic_read(&(my_mailbox->len)) == 0) {
            // up(&(my_semaphores_write[minor]));
            spin_unlock(&(my_mailbox->lock));
            WARNING_HELPER(1, "no message");
        }
    }

    DEBUG_INFO("qui 2");
    if (len < my_mailbox->head->len) {
        if (blocking_read) up(&(my_semaphores_read[minor]));
        spin_unlock(&(my_mailbox->lock));
        WARNING_HELPER(1, "dest buffer len too low");
    }


    // print_mailbox(my_mailbox);

    my_message = my_mailbox->head;
    l = my_message->len;

    // print_mail(my_message);
    char aux[l];
    int i;
    for (i = 0; i < l; i++) {
        aux[i] = my_message->txt[i];
    }



    DEBUG_INFO("qui 3");
    kfree(my_message->txt);
    head = my_mailbox->head->next;
    kfree(my_mailbox->head);
    my_mailbox->head = head;
    atomic_dec(&(my_mailbox->len));

    print_mailbox(my_mailbox);
    up(&(my_semaphores_write[minor]));
    spin_unlock(&(my_mailbox->lock));

    ret = copy_to_user(buff, aux, l);
    ERROR_HELPER(ret < 0, "copy_to_user failed");

    printk("%s: read end , minor=%d , pid=%d\n", MOD_NAME, minor, current->pid);
    return l;
}


static long mail_ioctl(struct file* filp, unsigned int cmd, unsigned long arg) {
    int minor = iminor(filp->f_path.dentry->d_inode);
    printk(KERN_INFO "%s IOctl:  minor=%d , pid=%d\n", MOD_NAME, minor, current->pid);
    printk("comando: %d\n", cmd);
    switch (cmd) {
        DEBUG_INFO("switch");
    case CMD_MAX_MSG:
        DEBUG_INFO("msg");
        if (arg < 0 || arg > MAX_MSG) return ERROR;
        DEBUG_INFO("ok");
        max_msg = arg;
        break;
    case CMD_MAX_MSG_LEN:
        DEBUG_INFO("len");
        if (arg < 0 || arg > MAX_MSG_LEN) return ERROR;
        DEBUG_INFO("ok");
        max_msg_len = arg;
        break;

    case CMD_BLOCKING_READ:
        DEBUG_INFO("read");
        if (arg != 0 && arg != 1) return ERROR;
        DEBUG_INFO("ok");
        blocking_read = arg;
        break;

    case CMD_BLOCKING_WRITE:
        DEBUG_INFO("write");
        if (arg != 0 && arg != 1) return ERROR;
        DEBUG_INFO("ok");
        blocking_write = arg;
        break;
    }
    return SUCCESS;
}


static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = mail_write,
    .open =  mail_open,
    .read =  mail_read,
    .release = mail_release,
    .unlocked_ioctl = mail_ioctl
};



int init_module(void) {
    int i;
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {
        printk("Registering hit device failed\n");
        return Major;
    }

    printk(KERN_INFO "Mail Slot device registered, it is assigned major number %d\n", Major);

    for (i = 0; i < MAX_INSTANCES; i++) {
        spin_lock_init(&(my_instances.list[i].lock));
        atomic_set(&(my_instances.list[i].len), 0);
        sema_init(&my_semaphores_read[i], 0);
        sema_init(&my_semaphores_write[i], max_msg);
    }

    atomic_set(&(my_instances.count), 0);
    DEBUG_INFO("fine init_module");
    return 0;
}


void cleanup_module(void) {
    int i, j;

    printk(KERN_INFO "Mail Slot device unregistered, it was assigned major number %d\n", Major);

    for (i = 0; i < MAX_INSTANCES; i++) {
        mailbox* my_mailbox = &(my_instances.list[i]);
        printk("box %p\n", my_mailbox);
        if (atomic_read(&(my_mailbox->len)) == 0) continue;
        mail* my_message = my_mailbox->head;
        printk("mail %p\n", my_message);
        mail* next = my_message->next;
        printk("next %p\n", next);
        for (j = 0; j < atomic_read(&(my_mailbox->len)); j++) {
            printk("txt %p\n", my_message->txt);
            printk("mail %p\n", my_message);
            kfree(my_message->txt);
            kfree(my_message);
            my_message = next;
            if (my_message != NULL) next = my_message->next;
            printk("mail %p\n", my_message);
            printk("next %p\n", next);
        }
    }
    unregister_chrdev(Major, DEVICE_NAME);
}
