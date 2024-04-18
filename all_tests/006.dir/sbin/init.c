#include "libc.h"

/*
This testcase tests the individual's open, close, and calls to cp (which also calls read and write), specifically
when opening multiple files in one execution flow. 
*/

int main(int argc, char** argv) {
    printf("*** We are starting the testcase!\n");
    int fd = open("/etc/data.txt", 0); //should open file with dune 2

    //failure while opening the file
    if (fd < 0) {
        printf("*** Failed to open file: /etc/data.txt\n");
        return -1;
    }

    printf("*** Reading from /etc/data.txt\n");
    printf("*** len = %d\n",len(fd)); //print byte length
    cp(fd, 1); //copies fd into the file descriptor (1) which will print it to the terminal (stout)
    printf("*** Finished reading!\n");
    close(fd);
    printf("***\n");


    int newfile = open("/etc/panic.txt", 0); //should open file with poor things

    //failure while opening the file
    if (newfile < 0) {
        printf("*** Failed to open file: /etc/panic.txt\n");
        return -1;
    }

    printf("*** Reading from /etc/panic.txt\n");
    printf("*** len = %d\n",len(newfile)); //print byte length
    cp(newfile, 1); //copies fd into the file descriptor (1) which will print it to the terminal (stout)
    printf("*** Finished reading!\n");
    close(newfile);


    shutdown();
    return 0;
}
