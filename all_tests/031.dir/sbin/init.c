#include "libc.h"

int main(int argc, char** argv) {
    /*Prints the given argc and argv*/
    printf("*** this is argc: %d\n",argc);
    for (int i = 0; i < argc; i++) {
        printf("*** this is argv: %s\n", argv[i]);
    }

    /*Testing opening files at a given path*/
    printf("*** TESTING OPENING FILES ***\n");
    int fd = open("/harry/welcome.txt",0);
    /*Tests the len system call*/
    printf("*** Length of this file = %d\n",len(fd));

    /*Makes sure the first available fd is where the file was placed*/
    if(fd != 3) {
        printf("Your open is returning the wrong fd!\n");
    }

    /*Prints the contents of the file if it was opened correctly*/
    cp(fd, 2);
    printf("*** \n");

    /*Testing the seek system call*/
    printf("*** TESTING SEEK ***\n");
    printf("*** Changing offset, should only print 2nd line now. This is what offset is changing by: %ld\n", seek(fd, 28));
    /*Prints the contents of the file if it was opened correctly*/
    cp(fd,2);
    printf("*** \n");

    /*Tests closing a file*/
    printf("*** TESTING CLOSING FILES ***\n");
    printf("*** close = %d\n", close(fd));
    printf("*** \n");

    /*Tests opening a new file after closing one*/
    printf("*** TESTING OPENING A NEW FILE ***\n");
    fd = open("/harry/moony.txt",0);
    if(fd != 3) {
        printf("Your open is returning the wrong fd! It should be at the fd our last file previously was.\n");
    }
    /*Prints the contents of the file if it was opened correctly*/
    cp(fd,2);
    printf("*** \n");

    /*Opening a new file*/
    printf("*** TESTING OPENING ANOTHER NEW FILE ***\n");
    fd = open("/dune/herbert.txt",0);
    /*Prints the contents of the file if it was opened correctly*/
    cp(fd,2);
    printf("*** \n");

    /*Testig fork system call*/
    printf("*** TESTING FORK ***\n");
    int id = fork();
    if(id < 0) {
        printf("*** Your fork is not working!\n");
    } else if(id == 0) {
        printf("*** I AM THE CHILD\n");
        execl("/sbin/shell", "lisan", "al", "gaib", 0);
        printf("*** I AM THE CHILD, and I should not have returned from execl.\n");
    } else {
        /*If 27 prints after wait it means fork was not executed properly or that the address space was not cleared*/
        uint32_t status = 27;
        wait(id,&status);
        /*Should print after the child print statements since we are waiting for the child*/
        printf("*** back from wait %ld\n",status);
        fd = open("/dune/data.txt",0);
        cp(fd,1);
    }

    shutdown();
    return 0;
}
