#include "libc.h"

int main(int argc, char** argv) {
    // Can you open files?
    int fd1 = open("/fortunes", 0);
    if (fd1 < 0) {
        // File does exit. fd1 should not be negative.
        printf("*** Failed to open file.\n");
        shutdown();
    }
    printf("*** Successfully opened file.\n");

    // This makes our next read start from an offset of 50.
    seek(fd1, 50);

    // Do we properly read from our files? Starting from the right offset?
    char buffer[10];
    read(fd1, buffer, 9);
    buffer[9] = 0;
    printf("*** %s\n", buffer);

    // No longer need the file.
    close(fd1);

    int id = fork();
    if (id < 0) {
        // Did not successfully fork.
        printf("*** Failed to fork.\n");
        shutdown();
    } else if (id == 0) {
        // We are the child process.
        int fd2 = open("/hello", 0);
        if (fd1 < 0) {
            printf("*** Failed to open file.\n");
            shutdown();
        }
        printf("*** Successfully opened file.\n");
        // Tests len. Are we properly finding the length of a file?
        printf("*** %d\n", len(fd2));
        exit(0);
    } else {
        // Wait for the child process. Prevents race conditions in print statements.
        uint32_t status = 42;
        wait(id, &status);
        printf("*** Child process complete.\n");

        // execl should return if file is not an ELF file.
        execl("/fortunes", "Not an ELF File", 0);
        printf("*** Sucessfuly returned from bad execl called.\n");

        // Are we creating semaphores correctly?
        int s1 = sem(1);

        // Parent decrements semaphore. Child cannot run until it increments it.
        down(s1);

        int child2 = fork();
        if (child2 == 0) {
            // Has to wait for semaphore to be incremented. Child work will print after
            // parent work.
            down(s1);
            for (int i = 0; i < 5; i++) {
                printf("*** Child work #%d\n", i + 1);
            }
            up(s1);
            exit(0);
        } else {
            for (int i = 0; i < 5; i++) {
                printf("*** Parent work #%d\n", i + 1);
            }

            // Allows child process to run and waits for it to complete/
            up(s1);
            uint32_t status = 42;
            wait(id, &status);
            printf("*** Child process complete.\n");
        }
    }
    shutdown();
    return 0;
}
