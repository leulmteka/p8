#include "libc.h"

// this testcases tests seek, read, and open
// inspired by t0 lol
int main(int argc, char** argv) {
    printf("*** here are some of my favorite things right now\n");

    printf("*** superheroes! ***\n");
    int fd = open("/data/superheroes.txt",0);
    cp(fd, 2);

    close(fd);
    printf("\n");
    fd = open("/data/food.txt",0);
    printf("*** open again \n");
    char buffer[100];
    uint32_t offset = seek(fd,30);
    // if you print out more than wraps and mac and cheese ball then your seek is wrong
    //printf("*** seek %ld\n",);
    read(fd, buffer, offset);
    printf("*** food!! ***\n");
    printf("***%s", buffer);
    printf("\n");
    
    char buffer2[100];
    fd = open("/data/movies.txt",0);
    printf("*** open x3 \n");
    printf("*** movies!! ***\n");
    offset = seek(fd, 13);
    // if you print out more than pitch perfect, your seek is wrong
    read(fd, buffer2, offset);
    printf("***%s", buffer2);
    printf("\n");
    printf("***done!!\n");

    shutdown();
    return 0;
}
