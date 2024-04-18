#include "libc.h"

int main(int argc, char *argv[]) {

    // warm up test for write syscall
    char p[6] = {'*', '*', '*', ' ', 't', '\n'};
    for (int i = 0; i < 6; i ++) {
        write(1, (p + i), 1);
    }

    // testing fork and execl
    int id = fork();    
    if (id < 0) { printf("bruh\n"); }
    else if (id == 0) { 
        execl("/sbin/readtest", "readtest", 0);
    } 
    return 0;
}
