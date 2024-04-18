#include "libc.h"

int main(int argc, char** argv) {
    printf("*** argc: %d\n",argc);
    
    int fd = open("moose2.txt", 0);
    char buffer[len(fd) + 1];

    buffer[len(fd)] = 0;
    read(fd, buffer, len(fd));
    printf("%s\n", buffer);

    return 101;

}