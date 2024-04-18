#include "libc.h"

/*void one(int fd) {
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));

    cp(fd,2);
}*/

int main(int argc, char** argv) {
    printf("*** %d\n",argc);
    for (int i=0; i<argc; i++) {
        printf("*** %s\n",argv[i]);
    }
    char buf1[7];
    // char buf2[300];
    char buf3[100];
    int fd1 = open("/etc/data.txt",0);
    int fd2 = open("/data/data.txt",0);
    // read(fd1, buf1, 7);
    // read(fd2, buf2, 1736);
    int numCharsPerLine[] = {11, 25, 25, 26, 28, 24, 31, 31};
    // int offset = seek(fd2, 0);
    // printf("*** back to offset: %d\n", offset);
    // int j = 0;
    int haveRead = 0;
    for(int i = 0; i < 7; i++){
        read(fd1, buf1, 7);
        printf("%s\n", buf1);
        // seek(fd1, 7);
        // int count = 0;
        // printf("before loop\n");
        
        // seek(fd2, j + 1);
        // seek(fd2, haveRead);
        int curRead = read(fd2, buf3, numCharsPerLine[i]);
        haveRead += curRead;
        printf("%s\n", buf3);
    }
    int curRead = read(fd2, buf3, numCharsPerLine[7]);
    haveRead += curRead;
    printf("%s\n", buf3);
    read(fd1, buf1, 7);
    printf("%s\n", buf1);
    // printf("%s\n", buf1);
    printf("*** have read %d bytes in second file\n", haveRead);
    printf("*** file 1 close = %d\n", close(fd1));
    printf("*** file 2 close = %d\n", close(fd2));

    shutdown();
    return 0;
}
