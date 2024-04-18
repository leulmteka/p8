#include "libc.h"

// this test case has two parts. It testes semaphores and fork.

// this test case checks to see if your semaphore system call implementation in sys.cc works correctly. 
// to pass this test case you will need working implementations of: case 3 (sem), case 4 (up), and case 5 (down) 
// if you fail the test case, double check your implementation of semaphores.

// the second part of the test cases tests a fork inside a fork as well as semaphores inside a fork
// Make sure that your fork() implemntation works for nested forks.



void one(int fd) {
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));

    cp(fd,2);
}

int main(int argc, char** argv) {
    // semaphore
    int my_sem = sem(3);

    // testing semaphore down
    if(down(my_sem)<0) {
        printf("*** semaphore down does not work. Failed.\n");
    } else {
        printf("*** semaphore down works. Success \n");
    }

    // testing semaphore up 
    if(up(my_sem)<0) {
        printf("*** semaphore up does not work. Failed.\n");
    } else {
        printf("*** semaphore up works. Success \n");
    }

    
    // Part two of the test case tests a fork inside a fork as well as semaphores inside a work

    // another semaphore
    int semaphore = sem(1);

    // first fork created
    int first_fork = fork();
    if (first_fork < 0) {
        printf("*** fork one failed\n");
    } else if (first_fork == 0) {
        /* child one */
        printf("*** in child one\n");

        // second fork created
        int second_fork = fork();
        if (second_fork < 0) {
            printf("*** fork two failed\n");
        } else if (second_fork == 0) {
            // testing semaphore inside of the child 
            down(semaphore);
            printf("*** in the second child\n");
            up(semaphore);
            printf("*** test for semaphore inside a fork passed.\n");
        } else {
            uint32_t status = 42;
            wait(second_fork,&status);
        }

    } else {
        /* parent */
        uint32_t status = 42;
        wait(first_fork,&status);
    }

    printf("*** Yay! Test passed.\n");

    shutdown();
    return 0;
}

