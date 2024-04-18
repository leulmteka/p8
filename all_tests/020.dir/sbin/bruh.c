#include "libc.h"

int main(int argc, char** argv) {
    printf("*** you have made it to bruh.c\n");
    int sem1 = sem(2);

    down(sem1);
    down(sem1);

    int fd = open("/etc/end.txt",0);
    
    char devious_buf[100];
    //write(fd, devious_buf, 50);
    read(fd, devious_buf, 100);

    printf("%s\n", devious_buf);
    
    up(sem1);

    return 0;
}