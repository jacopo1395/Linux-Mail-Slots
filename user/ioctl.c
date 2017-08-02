#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>


#include "../module/config.h"


int main() {
    char* dev="/dev/ms0\0";
    char* s="ciao\0";
    printf("start %s\n", dev);
    int fd = open(dev, O_RDWR);
    if (fd < 0) {
        printf("%s - err open\n", dev);
        return -1;
    }
    char data[100];

    int ret;
    int l= (int)strlen(s);


    ioctl(fd,CMD_MAX_MSG, 3);
    ioctl(fd,CMD_MAX_MSG_LEN, 5);
    ioctl(fd,CMD_BLOCKING_WRITE, 0);
    ioctl(fd,CMD_BLOCKING_READ, 0);
    ioctl(fd,CMD_MAX_MSG, 3);

    // ret = write(fd, s, l);
    // if (ret < 0) {
    //     printf("%s - err write\n", dev);
    //     return -1;
    // }
    // ret = write(fd, s, l);
    // if (ret < 0) {
    //     printf("%s - err write\n", dev);
    //     return -1;
    // }
    //
    // ioctl(fd,CMD_MAX_MSG, 5);
    //
    // ret = write(fd, s, l);
    // if (ret < 0) {
    //     printf("%s - err write\n", dev);
    //     return -1;
    // }

    close(fd);
    return 0;
}
