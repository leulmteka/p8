#include "libc.h"

int main(int argc, char** argv) {

    // basic reading and seeking
    int temple_fd = open("/data/temple.txt",0);
    char buffer[len(temple_fd) + 1];
    buffer[len(temple_fd)] = 0;
    read(temple_fd, buffer, len(temple_fd));
    printf("%s\n", buffer);
    printf("***\n");

    seek(temple_fd, 705);
    read(temple_fd, buffer, (len(temple_fd) - 705));
    printf("%s\n", buffer);
    printf("***\n");

    int trash_fd = open("/data/trash.txt",0);
    if (trash_fd != 4) {
        shutdown();
    }

    // attempts to corrupt kernel should not actually run
    seek(trash_fd, 0);
    void *fake_buffer = (void *) 0x00005000;
    int result = read(trash_fd, fake_buffer, len(trash_fd));
    if (result != -1) shutdown();
    
    // test making child from child
    // test to see if semaphores function properly
    int s1 = sem(0);
    int s2 = sem(0);

    int id = fork();

    if (id < 0) {
        printf("fork failed\n");
    } else if (id == 0) {
        printf("*** child\n");

        int id2 = fork();

        if (id2 < 0) {
            printf("fork failed\n");
        } else if (id2 == 0) {
            // grand child
            printf("*** grand child\n");
            up(s2);
            return 101;
        } else {
            // child
            down(s2);
            printf("*** child unlocked\n");
            up(s1);
        }
        return 101;
    } else {
        // parent should wait for both children
        down(s1);
        printf("*** parent unlocked\n");
        // int fd = open("/etc/panic.txt",0);
    }

    // test chaining: call a program that calls a program that calls another program
    int rc = execl("/sbin/p1","a","b","c",0);
    printf("execl failed: %d\n", rc);
    shutdown();
}
