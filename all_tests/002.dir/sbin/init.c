#include "libc.h"

/*
 * This test case tests the semaphore system calls by creating multiple
 * semaphores and coordinating the execution of multiple processes using them.
 * 
 * Due to the nature of testing semaphores, this test also tests your fork and wait functionalltiy
 * and ensures that process creation and synchronization work as expected.
 *
 * The idea here is to create three semaphores with different initial values
 * and have a parent process and two child processes interact with them in a
 * specific order to ensure that the semaphore operations work as expected.
 *
 * If the semaphore operations work correctly, the processes should execute in
 * the expected order, with child process 1 blocking initially, then child
 * process 2 blocking after acquiring sem_id2 twice, and finally the parent
 * process blocking after acquiring sem_id3 three times. Then, at the very end,
 * there are some checks that tests for proper error handling.
 */

int main(int argc, char** argv) {
    int sem_id1 = sem(0);
    int sem_id2 = sem(1);
    int sem_id3 = sem(2);

    if (sem_id1 < 0 || sem_id2 < 0 || sem_id3 < 0) {
        printf("*** Failed to create semaphores\n");
        return 1;
    }

    int child_id1 = fork();
    if (child_id1 < 0) {
        printf("*** Fork failed\n");
        return 1;
    } else if (child_id1 == 0) {
        down(sem_id1);
        printf("*** Child process 1 started\n");
        printf("*** Child process 1 doing some work\n");
        for (int i = 0; i < 3; i++) {
            printf("*** Child process 1 working: %d\n", i);
        }
        printf("*** Child process 1 finished, releasing semaphore 2\n");
        up(sem_id2);
        exit(0);
    }

    int child_id2 = fork();
    if (child_id2 < 0) {
        printf("*** Fork failed\n");
        return 1;
    } else if (child_id2 == 0) {
        // since initial value is 1, we decrease the value by 2
        down(sem_id2);
        down(sem_id2);
        printf("*** Child process 2 started\n");
        printf("*** Child process 2 doing some work\n");
        for (int i = 0; i < 4; i++) {
            printf("*** Child process 2 working: %d\n", i);
        }
        printf("*** Child process 2 finished, releasing semaphore 3\n");
        up(sem_id3);
        exit(0);
    }

    printf("*** Parent process started\n");
    printf("*** Parent process doing some work\n");
    for (int i = 0; i < 5; i++) {
        printf("*** Parent process working: %d\n", i);
    }
    printf("*** Parent process releasing semaphore 1\n");
    up(sem_id1);

    // since initial value is 2, we decrease the value by 3
    down(sem_id3);
    down(sem_id3);
    down(sem_id3);

    uint32_t status1, status2;
    wait(child_id1, &status1);
    printf("*** Parent process finished waiting for child process 1, status: %d\n", (int)status1);
    wait(child_id2, &status2);
    printf("*** Parent process finished waiting for child process 2, status: %d\n", (int)status2);

    printf("*** Parent process finished\n");


    // check for semaphore errors


    printf("*** Creating more semaphores to check for proper error return value\n");

    int errorInt = 0;
    for(int i = 0; i < 8; i++){
        errorInt = sem(0);
    }
    if(errorInt > 0){
        printf("*** Error return value for creating many semaphores is > 0: %d\n", errorInt);
    }
    printf("*** Error return value for creating many semaphores is < 0: %d\n", errorInt);


    int downError = down(errorInt);
    if(downError == 0){
        printf("*** Error return value for down is 0 which indicates a success: %d\n", downError);
    }else if(downError < 0){
        printf("*** Error return value for down is < 0 which indicates a failure: %d\n", downError);
    }

    int upError = up(errorInt);
    if(upError == 0){
        printf("*** Error return value for up is 0 which indicates a success: %d\n", upError);
    }else if(upError < 0){
        printf("*** Error return value for up is < 0 which indicates a failure: %d\n", upError);
    }

    shutdown();
    return 0;
}