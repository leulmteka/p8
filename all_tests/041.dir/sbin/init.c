#include "libc.h"

void one(int fd) {
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));

    //cp(fd,2);
}

int main(int argc, char** argv) {
    int fdData = open("/etc/data.txt",0);
        one(fdData);
    printf("*** close = %d\n",close(fdData));

    int fdPanic = open("/etc/panic.txt",0);
        one(fdPanic);
    printf("*** close = %d\n",close(fdPanic));







    shutdown();
    return 0;
}
