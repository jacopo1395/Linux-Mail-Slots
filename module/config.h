#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG 1 //0=false  1=true  | enable debug print

// NOT CHANGE!
#define SUCCESS 0         // const
#define ERROR -1          // const
#define MAX_INSTANCES 256 // const

//default values
#define MAX_MSG_LEN     50      // this is also the upper bound
#define MAX_MSG         512     // this is also the upper bound
#define BLOCKING_READ   0
#define BLOCKING_WRITE  0

//variables - deafault values
int max_msg_len = MAX_MSG_LEN;
int max_msg = MAX_MSG;
int blocking_read = BLOCKING_READ;
int blocking_write = BLOCKING_WRITE;


// ioctl commands
#define CMD_MAX_MSG_LEN     1
#define CMD_MAX_MSG         3  // 2 doesn't work!
#define CMD_BLOCKING_READ   4
#define CMD_BLOCKING_WRITE  5


#endif
