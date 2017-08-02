#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>


#include "../module/config.h"

#define N 6
#define M 2
int blocking = 1;

void *write_fun(void *args) {
    int fd = *(int*)args;
    int i;
    for (i=0; i<M; i++) {
        write(fd, "ciao", 4);
        // sleep(1);
    }
}

void *read_fun(void* args) {
    int fd = *(int*)args;
    char data[512];
    int i, ret;
    for (i=0; i<M; i++) {
        ret=read(fd, data, 512);
        // sleep(1);
        data[ret]='\0';

        printf("letto: %s\n", data);
    }
}


int main(int argc, char const *argv[]) {
    int i;

    if (argc > 1) blocking = 0;

    int fd = open("/dev/ms0", O_RDWR);
    if(fd < 0)
        return 1;



    pthread_t threads_write[N];
    pthread_t threads_read[N];
    for (i=0; i<N; i++) {
        if(pthread_create(&threads_write[i], NULL, write_fun, &fd)) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
        // if(pthread_create(&threads_read[i], NULL, read_fun, &fd)) {
        //     fprintf(stderr, "Error creating thread\n");
        //     return 1;
        // }
    }


    for (i=0; i<N; i++) {
        if(pthread_join(threads_write[i], NULL)) {
            fprintf(stderr, "Error joining thread\n");
            return 2;
        }
        // if(pthread_join(threads_read[i], NULL)) {
        //     fprintf(stderr, "Error joining thread\n");
        //     return 2;
        // }
    }


    close(fd);
    return 0;
}
