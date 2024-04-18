#include "libc.h"

int main(int argc, char** argv) {

    int fd = open("/top10cs439moments", 0);
    char buffer[33];
    read(fd, buffer, 33);
    printf("*** Worst CS439 Moment: %s\n", buffer);

    // checks to make sure this doesnt impact the parent process
    seek(fd, 96);
    
    // close(fd); //not closing file just in case

    return argv[0][0] - '0';
}
