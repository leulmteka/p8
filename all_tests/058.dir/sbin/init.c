#include "libc.h"


// some extensive testing of fd and len
void one(int fd) {
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));

    printf("*** GOING INTO CP\n");
    cp(fd,2);
    printf("*** RETURNING FROM CP\n");

    // the following fd and len should be the same as the above fd and len
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));
}

int main(int argc, char** argv) {
    printf("*** SOME FLAG TO TELL ME THIS IS MY TEST\n");

    printf("*** %d\n",argc);
    for (int i=0; i<argc; i++) {
        printf("*** %s\n",argv[i]);
    }
    int fd = open("/etc/data.txt",0);
    one(fd);

    printf("*** close = %d\n",close(fd));

    // we just closed fd, the following will not work
    one(fd);
    // 99 is not defined
    one(99);
    // -1 is not defined
    one(-1);

    printf("*** open again %d\n",open("/etc/data.txt",0));
    // fd is now open
    one(fd);

    shutdown();
    return 0;
}
