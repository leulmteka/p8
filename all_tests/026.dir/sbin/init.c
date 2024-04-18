#include "libc.h"

// This test case tests semaphores, fork, and wait
// Created a small story of a parent asking their children
// what the want to be in the future

int main(int argc, char** argv) {
    // create three different semaphores
    int sem1 = sem(2);
    int sem2 = sem(2);
    int sem3 = sem(2);

    // check if semaphores were initialized correctly
    if (sem1 < 0 || sem2 < 0 || sem3 < 0) {
        printf("*** Did not create semaphores correctly\n");
        return 1;
    }
    printf("*** Parent asks kids what they wanna be when they grow up.\n");

    int child1 = fork();
    if (child1 < 0) {
        printf("*** fork failed\n");
        return 1;
    } else if (child1 == 0) {
        // since i initialized it to 2 i call down 3 times
        down(sem1);
        down(sem1);
        down(sem1);
        printf("*** Child 1's response:\n");
        printf("*** I want to be an astronaut\n");
        printf("*** Brother 2 what do you want to be?\n");
        // go to child2
        up(sem2);
        exit(0);
    }
    
    int child2 = fork();
    if (child2 < 0) {
        printf("*** fork failed\n");
        return 1;
    } else if (child2 == 0) {
        down(sem2);
        down(sem2);
        down(sem2);
        printf("*** Child 2's response:\n");
        printf("*** I want to be a firefighter\n");
        printf("*** Brother 3 what do you want to be?\n");
        // go to child3
        up(sem3);
        exit(0);
    }

    int child3 = fork();
    if (child3 < 0) {
        printf("*** fork failed\n");
        return 1;
    } else if (child3 == 0) {
        down(sem3);
        down(sem3);
        down(sem3);
        printf("*** Child 3's response:\n");
        printf("*** I want to be a software engineer,\n");
        printf("*** how hard can it be?\n");
        exit(0);
    }

    printf("*** Child 1 what do you want to be?\n");
    up(sem1);

    // wait for child processes to finish
    uint32_t status1, status2, status3;
    wait(child1, &status1);
    wait(child2, &status2);
    wait(child3, &status3);
    printf("*** Parent: ... oh boy\n");

    // end program
    shutdown();
    return 0;
}