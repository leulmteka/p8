#include "libc.h"

/*
* This test case first checks you implementation of the sem, up, and down system calls as well 
* as fork, wait, and execl.
*
* Next, it tests your open functions to ensure you can properly open files, and that you your
* implementation of the open system call properly handles files that do not exist.
* 
* Lastly, it tests that a user cannot successfully call read and write on kernel space.
*/

int main(int argc, char** argv) {
    // first create semaphores 
    int semaphore = sem(1);

    // check to ensure you were able to successfully create sempahores (sem works)
    if (semaphore < 0) {
        printf("*** could not successfully create semaphore, check your sem system call\n");
        return -1;
    }

    printf("*** Yay! We have semaphores!\n");
    down(semaphore);

    // test fork
    int child = fork();

    // test to check if a fork was successful
    if (child < 0) {
        printf("*** could not successfully create a child, check your fork system call\n");
        return -1;
    }

    if (child == 0) { // we are running the child process
        down(semaphore); // child process should be blocked until semaphore goes back up
        printf("*** We should be in the child now...\n");
        int rc = execl("/sbin/shell", "shell", "a", "b", "c", 0);
        printf("*** execl failed, rc = %d\n", rc);
        return -1;
    }

    else { // we are the parent process
        printf("*** Starting in the parent\n");
        up(semaphore);

        // make the parent wait for the child to finish
        uint32_t status = 42;
        wait(child, &status);
        printf("*** Back to the parent now %ld\n", status);
    }

    printf("*** OKKKKKKKKKK Time to Test Open\n");

    int fd_1 = open("/etc/data.txt",0);

    if (fd_1 != 3) {
        printf("*** Uhoh something is wrong with open :(\n");
        return -1;
    }

    printf("*** Yay! Open wasn't a flop....hopefully\n");
    printf("*** Time to toughen things up a bit >:)\n");

    // tries to open a file that does not exist in our file system
    int file_id = open("/etc/doesnt_exist.txt", 0);
    if (file_id >= 3) {
        printf("*** Trying to open something that doesn't exist? Weird but ok\n");
        return -1;
    }

    printf("*** Yay! You didn't open a non-existent file!\n");

    printf("*** Now for the true test...can the user read and alter space they should't?\n");

    int fd_2 = open("/data/panic.txt", 0);

    // test if the user can read from kernel space
    int read_results = read(fd_2, (uint32_t*) 0x00001000, len(fd_2));

    if (read_results >= 0) {
        printf("*** Should the user read this?\n");
        return -1;
    }

    // test if the user can write/alter kernel space
    int write_results = write(0, (uint32_t*) 0x00001000, len(fd_2));
    if (write_results >= 0) {
        printf("*** Should the user alter this space?\n");
        return -1;
    }

    printf("*** Yay! You made it all the way through!\n");

    shutdown();
    return 0;
}
