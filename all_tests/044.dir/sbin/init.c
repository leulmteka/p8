#include "libc.h"

/*

    This test case mainly tests semaphore synchronization, process creation with the fork method, and process execution with the execl() method.

    Main Tests:
        1.) File operations with open, read, and close
        2.) Process Manegment with fork, wait, execl, and exit
        3.) Semaphore operations with sem, down, and up

    If you're failing this test case, it may be due to the following reasons:
        1.) Improper implementation of semaphores (down and up) if you're having deadlocks
        2.) Improper implementation of reading and closing files if you're having resource leaks or if your output does not match the .ok file
        3.) Improper implementation of process creation and termination if you cannot execute the child process

*/

int main() {
    printf("*** Welcome to my test case\n");
    // We open greeting.txt
    int fd = open("/data/greeting.txt", 0);
    char buffer[256];
    // Read the file
    read(fd, buffer, 255);
    buffer[255] = '\0';
    // Output the contents of greeting.txt
    printf("%s\n", buffer);
    // Free file descriptor
    close(fd);

    // We create a semaphore and test it's operations with down and up
    printf("*** Initializing Semaphore\n");
    int sem_id = sem(2);
    down(sem_id);
    down(sem_id);

    // Start a child operation to test the implementation of fork method
    printf("*** Fork Test Start\n");
    int child_number = fork();
    if (child_number == 0) {
        up(sem_id);
        // Execute the child process
        int rc = execl("/sbin/shell", "shell", 0);
        printf("*** execl failed, rc = %d\n", rc);
        exit(1);
    } else {
        // Here we wait for the child process (shell.c) to be done
        uint32_t done;
        wait(child_number, &done);
        printf("*** Child process finished with status %ld\n", done);
    }

    down(sem_id);

    printf("*** Semaphore final status: 0\n");
    // Terminate
    shutdown();
    return 0;
}
