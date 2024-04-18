#include "libc.h"

// testing open and read
int main(int argc, char** argv) {
    char stars[2] = {'*', ' '};
    for (int i = 0; i < 3; i++) write(1, stars, 1);
    write(1, stars + 1, 1);
    int fd = open("/manifold.txt", 0);
    char buf[10];
    read(fd, buf, 10);
    for (int k = 0; k < 10; k++) {
        write(1, buf+k, 1);
    }
    
    char newline = '\n';
    write(1, &newline, 1);
    return 0;
}
