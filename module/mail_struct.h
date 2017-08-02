#ifndef MAIL_STRUCT_H
#define MAIL_STRUCT_H

#include "config.h"

typedef struct node {
    // mail* prev;
    struct node* next;
    char* txt;
    int len;
} mail;


typedef struct list {
    mail* head;
    mail* tail;
    atomic_t len;
    spinlock_t lock;
} mailbox;


typedef struct instances{
    mailbox list[MAX_INSTANCES];
    atomic_t count;
}instances;

#endif 
