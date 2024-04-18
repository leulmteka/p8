#include "libc.h"

//This test case tests if you can successfully open read and write files.


//I stole this from gheith 
void one(int fd) {
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));

    cp(fd,2);
}

int main(int argc, char** argv) {
    printf("*** %d\n",argc);
    for (int i=0; i<argc; i++) {
        printf("*** %s\n",argv[i]);
    }
    //Test if you can open file 1
    int fd = open("/part1.txt",0);
    one(fd);

    printf("*** close = %d\n",close(fd));

    //Test if you can open file 2
    fd = open("/part2.txt", 0);
    one(fd);
    printf("*** close = %d\n", close(fd));

    //Test if you can open file 3
    fd = open("/part3.txt", 0);
    one(fd);
    printf("*** close = %d\n", close(fd));

    //Test if you can open file 4
    fd = open("/part4.txt", 0);
    one(fd);
    printf("*** close = %d\n", close(fd));

    shutdown();
    return 0;
}
