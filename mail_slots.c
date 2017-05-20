
/*  
 *  broadcast dev file driver - you can broadcast streams of bytes on all the registered ttys 
 *  by simply writing to the broadcast dev file
 *  preliminary you need to mknod the dev file and assign the major retrived while mounting this module
 */

#define EXPORT_SYMTAB
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
	
#include <linux/pid.h>		/* For pid types */
#include <linux/tty.h>		/* For the tty declarations */
#include <linux/version.h>	/* For LINUX_VERSION_CODE */


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jacopo Carlini");



static int mail_open(struct inode *, struct file *);
static int mail_release(struct inode *, struct file *);
static ssize_t mail_write(struct file *, const char *, size_t, loff_t *);

#define DEVICE_NAME "mail"  /* Device file name in /dev/ - not mandatory  */


static int Major;            /* Major number assigned to broadcast device driver */


/* the actual driver */


static int mail_open(struct inode *inode, struct file *file)
{

//device opened by a default nop
   return 0;
}


static int mail_release(struct inode *inode, struct file *file)
{

//device closed by default nop
   return 0;

}




static ssize_t mail_write(struct file *filp, const char *buff, size_t len, loff_t *off){   
  return len;
}



static struct file_operations fops = {
  .write = mail_write,
  .open =  mail_open,
  .release = mail_release
};



int init_module(void)
{

	Major = register_chrdev(0, DEVICE_NAME, &fops);

	if (Major < 0) {
	  printk("Registering hit device failed\n");
	  return Major;
	}

	printk(KERN_INFO "Mail Slots device registered, it is assigned major number %d\n", Major);


	return 0;
}

void cleanup_module(void)
{

	unregister_chrdev(Major, DEVICE_NAME);

	printk(KERN_INFO "Mail Slots device unregistered, it was assigned major number %d\n", Major);
}
