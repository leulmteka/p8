#include "libc.h"

/*
    This test case closes fd 0 and 2 and stress tests all child processes being queued.
    This is tested by putting all of the children in the thunderdome.

    An additional test at the end makes sure that close functions with closing files, semaphores,
    and children.
*/

uint32_t children_done = 9;

int main(int argc, char** argv) {
    printf("*** start\n");

    int rc1 = close(0);
    int rc3 = close(2);

    if (rc1 == -1 || rc3 == -1) {
        printf("Hey wait, this is a legal move!\n");
        shutdown();
    }
    printf("*** passed closing fd 0 and 2 test!\n");


    /* open all files, filling up the fd */

    int fd1 = open("/thunderdome/challenger_1", 0);
    int fd2 = open("/thunderdome/challenger_2", 0);

    if (fd1 != 0 || fd2 != 2) {
        printf("opened files did not take the place of std files\n");
        shutdown();
    }
    printf("*** passed new files in fd 0 and 2 test!\n");

    open("/thunderdome/challenger_3", 0);
    open("/thunderdome/challenger_4", 0);
    open("/thunderdome/challenger_5", 0);
    open("/thunderdome/challenger_6", 0);
    open("/thunderdome/challenger_7", 0);
    open("/thunderdome/challenger_8", 0);
    open("/thunderdome/challenger_9", 0);

    // file table now full, making sure adding another one returns an error

    int fderr = open("/thunderdome/challenger_9", 0);
    if (fderr != -1) {
        printf("Non-error valued returned from opening past 10 files\n");
        shutdown();
    }
    printf("*** passed full fdt test!\n");

     printf("*** Let the games begin...\n");

    int sem_id = sem(1);
    down(sem_id); // allow parent to control process flow
    int children[10];
    /* process stress test */
    for (int i = 0; i <= 9; i++) {
        if (i == 1) continue; // skips the remaining std file
        int id = fork();
        children[i] = id;
        if (id == 0) {
            down(sem_id);
            char* ptr = malloc(10);
            read(i, (void*)ptr, 10);
            printf("%s\n", ptr);
            exit(0);
        }
    }

    for (int i = 0; i <= 9; i++) {
        if (i == 1) continue; // mirror the other loop's skip
        up(sem_id);
        uint32_t status = 0;
        wait(children[i], &status);
    }

    printf("*** Passed children stress test!\n");

    // Make sure close works on files, sems, and children

    int id = fork();
    if (id == 0) while(1);

    int rc = close(id);
    if (rc != 0) {
        printf("Child did not close\n");
        shutdown();
    }

    printf("*** close(child_id) test passed!\n");

    for (int i = 0; i <= 9; i++) {
        if (i == 1) continue;
        rc = close(i);
        if (rc != 0) {
            printf("File %d did not close\n", i);
            shutdown();
        }
    }

    printf("*** close(file_id) test passed!\n");

    rc = close(sem_id);
    if (rc != 0) {
        printf("sem did not close\n");
        shutdown();
    }

    printf("*** close(sem_id) test passed!\n");

    // call close on random values to ensure values correctly erased
    int semrc = close(sem_id);
    int childrc = close(id);
    int filerc = close(0);

    if (semrc != -1 || childrc != -1 || filerc != -1) {
        printf("one rc of the three are not -1: semrc: %d, childrc: %d, filerc: %d\n", semrc, childrc, filerc);
        shutdown();
    }

    printf("*** close failure test passed!\n");
    
    shutdown();
    return 0;
}

