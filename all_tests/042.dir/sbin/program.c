#include "libc.h"

int main(int argc, char** argv) {
    if (argc != 6) {
        printf("*** WRONG ARGC VALUE.\n");
        return -1;
    }
    int fd = open("/data/sample.txt", 0);
    cp(fd, 1); //copy file to stdout
    printf("*** Child %s\n", argv[1]);
    return 777; //Jackpot!
}
