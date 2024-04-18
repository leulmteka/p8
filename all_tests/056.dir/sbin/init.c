#include "libc.h"


int main(int argc, char** argv) {
    printf("*** %d\n",argc);
    for (int i=0; i<argc; i++) {
        printf("*** %s\n",argv[i]);
    }

    //Goes until the first fd is invalid
    for(int j = 0 ; j < 8; j++){
        int fd = open("/etc/data.txt",j);
        printf("*** fd counter = %d\n",fd);
        printf("*** length of file = %d\n",len(fd));
        cp(fd,2);
    }
    shutdown();
    return 0;
}
