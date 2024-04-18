#include "libc.h"

int main(int argc, char** argv) {
    printf("*** argc = %d\n",argc);
    for (int i=0; i<argc; i++) {
        printf("*** argv[%d]=%s\n",i,argv[i]);
    }

    // you CAN close these!
    close(0);
    close(2);

    int fd = open("/guh/bee.txt",0);
    if (fd != 0) {
        printf("*** You know you can close the 0th file descriptor, right?\n");
        shutdown();
    }

    int fd2 = open("/guh/bee.txt", 0);
    if (fd2 != 2) {
        printf("*** You know you can close the 2nd file descriptor, right?\n");
        shutdown();
    }

    cp(3, 1);

    int success = close(1);
    if (success == -1) {
        printf("*** You know you can close the 1st file descriptor, right?\n");
        shutdown();
    }

    // finally, since i closed stdout, this shouldn't return any text whatsoever.
    cp(0, 1);

    return 420;
}
