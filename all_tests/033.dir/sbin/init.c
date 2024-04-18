#include "libc.h"

// this test checks that read(), write(), execl(), and wait() sanatizes pointers
// i.e. all pointers passed in should point to memory that the user program is allowed to access.

int main(int argc, char** argv) {
    printf("*** read() Security Check\n");
    int fd = open("/fortunes", 0);
    ssize_t n1 = read(fd, (void *) 0x00100000, len(fd));
    if(n1 != -1){
        printf("*** read() is insecure, must ensure buf pointer is safe address to write to\n");
        shutdown();
    }
    printf("*** read() is secure\n");
    printf("*** write() Security Check\n");
    ssize_t n2 = write(2, (void *) 0x00100000, 4);
    if(n2 != -1){
        printf("*** write() is insecure, must ensure buf pointer is safe address to read from\n");
        shutdown();
    }
    printf("*** write() is secure\n");
    printf("*** execl() Security Check\n");
    // these should all fail, if they don't execl will run and delete the private space and the program will hang
    execl("/sbin/shell", (void *) 0x00100000, 0);
    execl("/sbin/shell", "a" , (void *) 0x00100000, 0);
    execl("/sbin/shell", "a", "b", (void *) 0x00100000, 0);
    execl("/sbin/shell", "a", "b", "c", (void *) 0x00100000, 0);
    printf("*** execl() is secure\n");
    printf("*** wait() Security Check\n");
    int id = fork();
    if(id < 0){
        printf("*** fork() failed\n");
        shutdown();
    }else if (id == 0){
        execl("/sbin/shell", 0);
    }else {
        uint32_t status = 42;
        if(wait(id, (void *) 0x00100000) != -1){
            printf("*** wait() is insecure\n");
            shutdown();
        }
        wait(id, &status);
        printf("*** wait() is secure\n");
    }
    printf("*** Good Job!\n");
    shutdown();
    return 0;
}
