#include "libc.h"

int main(int argc, char** argv) {
    printf("*** program 2, args: %d\n", argc);
    
    int fd = open("/data/trash.txt", 0);
    uint32_t length = len(fd) / 8;
    char buffer[length + 1];
    buffer[length] = 0;
    seek(fd, length + 1);
    read(fd, buffer, length);
    printf("%s\n", buffer);
    printf("*** \n");

    int rc = execl("/sbin/p3","shell",0);
    printf("p2 execl failed: %d\n", rc);
}