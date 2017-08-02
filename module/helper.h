#ifndef HELPER_H
#define HELPER_H

#define ERROR_HELPER(cond, msg) {                             \
    do {                                                      \
        if (cond) {                                           \
            printk(KERN_ERR "%s error: %s\n", MOD_NAME, msg); \
            return ERROR;                                     \
        }                                                     \
    } while (0);                                              \
}                                                             \

#define WARNING_HELPER(cond, msg) {                                 \
    do {                                                            \
        if (cond) {                                                 \
            printk(KERN_WARNING "%s warning: %s\n", MOD_NAME, msg); \
            return SUCCESS;                                         \
        }                                                           \
    } while (0);                                                    \
}                                                                   \

#define DEBUG_INFO(info) {                                        \
    do {                                                          \
        if (DEBUG) {                                              \
            printk(KERN_DEBUG "%s debug: %s \n", MOD_NAME, info); \
        }                                                         \
    } while (0);                                                  \
}                                                                 \



void print_mail(mail* m){
    int l, i;
    if(!DEBUG) return;
    if(m==NULL) return;
    l=m->len;
    char s[l+1];

    for (i = 0; i < l; i++) {
            /* code */
            s[i]=m->txt[i];
    }
    s[l]='\0';
    printk( KERN_DEBUG "-------------MAIL-------------\n");
    printk( KERN_DEBUG "mail->next = %p\n", m->next);
    printk( KERN_DEBUG "mail->txt = %s\n", s);
    printk( KERN_DEBUG "mail->len = %d\n", m->len);
    printk( KERN_DEBUG "-------------End---------------\n");
}

void print_mailbox(mailbox* b){
    int i;
    mail* m;
    if(!DEBUG) return;
    if(b==NULL) return;
    printk("\n+++++++++++MAILBOX++++++++++++\n");
    printk( KERN_DEBUG "mailbox->head = %p\n", b->head);
    printk( KERN_DEBUG "mailbox->len = %d\n", atomic_read(&(b->len)));

    m=b->head;
    for(i=0; i<atomic_read(&(b->len)); i++) {
            print_mail(m);
            m=m->next;
    }
    printk("++++++++++++++End+++++++++++++\n\n");

}

#endif
