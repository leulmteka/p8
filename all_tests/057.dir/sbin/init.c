#include "libc.h"

void test(int fd) {
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));

    cp(fd,2);
}

void test2(int fd) {
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));

    cp(fd,5);
}

int main(int argc, char** argv) {
    // printf("*** %d\n",argc);
    // for (int i=0; i<argc; i++) {
    //     printf("*** %s\n",argv[i]);
    // }
    int fd = open("/etc/data.txt",0);
    test(fd);

    printf("*** TESTING SEEK\n");
    printf("*** Seek: %ld\n", seek(fd, 0));

    // test2(fd);

    shutdown();
    return 0;
}
