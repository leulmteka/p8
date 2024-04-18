#include "libc.h"

int main(int argc, char** argv) {
    printf("*** program 1, args: %d\n", argc);
    
    int fd = open("/data/trash.txt", 0);
    uint32_t length = len(fd) / 8;
    char buffer[length + 1];
    buffer[length] = 0;
    read(fd, buffer, length);
    printf("%s\n", buffer);
    printf("*** \n");

    int rc = execl("/sbin/p2","1","2","3","4",0);
    printf("p1 execl failed: %d\n", rc);

}