#include "libc.h"

void one(int fd) {
    cp(fd,2);
    printf("***\n");
    printf("*** file descriptor = %d\n",fd);
    printf("*** length = %d\n",len(fd));
}

// The purpose of this testcase is to test the wrapper file class and make sure your offsets are
// independent regardless of proccesses. It opens the same file in the child and parent proccess,
// but should read the same thing

int main(int argc, char** argv) {
    printf("*** Things I love about\n");

    int fd = open("/cs439", 0);
    one(fd);
    printf("*** close = %d\n",close(fd));
    
    int id = fork();

    if (id < 0) {
        printf("*** fork failed");
    } else if (id == 0) {
        /* child */
        printf("*** in child\n");
        int rc = execl("/sbin/moments", "1",0);
        printf("*** execl failed, rc = %d\n",rc);
    } else {
        /* parent */
        uint32_t status = 42;
        wait(id,&status);
        printf("*** Child process finished. Wrapper class' offset should not be affected\n");

        int fd = open("/top10cs439moments", 0);
        char buffer[33];
        read(fd, buffer, 33);
        printf("*** Worst CS439 Moment is always the same: %s\n", buffer);

        close(fd);
    }

    shutdown();
    return 0;
}
