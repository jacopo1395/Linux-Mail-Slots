#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include "../module/config.h"

int msg(char* dev, char* s) {
    printf("start %s\n", dev);
    int fd = open(dev, O_RDWR);
    if (fd < 0) {
        printf("%s - err open\n", dev);
        return -1;
    }
    // char data[100];
    //
    // int ret;
    // int l= (int)strlen(s);
    // ret = write(fd, s, l);
    // if (ret < 0) {
    //     printf("%s - err write\n", dev);
    //     return -1;
    // }

    // ret = read(fd, data, 100);
    // if (ret < 0) {
    //     printf("%s - err read\n", dev);
    //     return -1;
    // }
    // data[l]='\0';
    //
    // printf("%s\n", data);

    close(fd);
}

int msgw(char* dev, char* s) {
    printf("start %s\n", dev);
    int fd = open(dev, O_RDWR);
    if (fd < 0) {
        printf("%s - err open\n", dev);
        return -1;
    }
    char data[100];

    int ret;
    int l= (int)strlen(s);
    ret = write(fd, s, l);
    if (ret < 0) {
        printf("%s - err write\n", dev);
        return -1;
    }


    close(fd);
}
int msgr(char* dev) {
    printf("start %s\n", dev);
    int fd = open(dev, O_RDWR);
    if (fd < 0) {
        printf("%s - err open\n", dev);
        return -1;
    }
    char data[100];

    int ret;


    ret = read(fd, data, 100);
    if (ret < 0) {
        printf("%s - err read\n", dev);
        return -1;
    }
    data[ret]='\0';

    printf("letto: %s\n", data);

    close(fd);
}


int main() {
    // int ret = fork();
    //msg("/dev/ms0\0","ciao\0");
    // msgw("/dev/ms0\0", "ciao\0");
    // msgw("/dev/ms0\0", "io tutto bene\0");
    // msgw("/dev/ms0\0", "tu?\0");
    // msgw("/dev/ms0\0", "ciao\0");
    // msgw("/dev/ms0\0", "io tutto bene\0");
    // msgw("/dev/ms0\0", "tu?\0");
    msgr("/dev/ms0\0");
    msgr("/dev/ms0\0");
    msgr("/dev/ms0\0");
    // if (ret) msg("/dev/ms0\0", "ciao\0" );
    // else msg("/dev/ms1\0", "hello!\0");
    // msgw("/dev/ms0\0", "ciao\0" );
    // msgr("/dev/ms1\0", "ciao\0");
    // msgr("/dev/ms0\0", "ciao\0");
    return 0;
}
