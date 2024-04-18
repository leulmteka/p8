#include "libc.h"

//this test case will genereally test your open, read, seek, and semaphores

int main(int argc, char** argv) {
    printf("*** argc Val:%d\n",argc);
    for (int i=0; i<argc; i++) {
        printf("*** argv Val:%s\n",argv[i]);
    }
    printf("*** open and close tests\n");
    int fd1 = open("/beforeandafter",0);
    printf("*** fd1 = %d\n",fd1);
    int fd2 = open("/random",0);
    printf("*** fd2 = %d\n",fd2);
    int fd3 = open("/beforeandafter/before/sad.txt",0);
    printf("*** fd3 = %d\n",fd3);


    printf("*** close = %d\n",close(fd1));
    printf("*** close = %d\n",close(fd2));
    printf("*** close = %d\n",close(fd3));

    printf("*** done with those!\n");
    printf("*** quick test of len \n***len = %d\n",len(50));

    printf("*** testing read \n");
    int sad = open("/beforeandafter/before/sad.txt",0);
    char readBuffer1[len(sad) + 1];
    read(sad, readBuffer1, len(sad));
    printf("%s\n", readBuffer1);
    int happy = open("/beforeandafter/after/happy.txt",0);
    char readBuffer2[len(happy) + 1];
    read(happy, readBuffer2, len(sad));
    printf("%s\n", readBuffer2);
    int errorCheck = read(happy, (void*) 0, 1);
    printf("*** error test result:%d \n", errorCheck);
    printf("*** done with read test! \n");


    printf("*** testing seek now \n");
    printf("*** seek1: %ld\n",seek(sad,4));
    printf("*** seek2 %ld\n",seek(happy,2));
    printf("*** seek test done \n");

    printf("*** Starting semaphore tests\n");

    // Create a semaphore
    int sem_id = sem(1); 
    if(sem_id < 0) {
        printf("*** Failed to create semaphore\n");
    }

    int pid = fork();
    if(pid == 0) { // Child process
        printf("*** Child process waiting on semaphore\n");
        down(sem_id);
        printf("*** Child process acquired semaphore\n");
        // Simulate some work
        for(int i = 0; i < 2; i++) {
            printf("*** Child process working %d\n", i);
        }
        up(sem_id);
        printf("*** Child process released semaphore\n");
        exit(0);
    } else if(pid > 0) { // Parent process

        uint32_t status;
        wait(pid, &status);
        
        printf("*** Parent process working before semaphore\n");
        int timer;
        for(int i = 0; i < 60; i++) {
            timer++;
        }
        down(sem_id);
        printf("*** Parent process acquired semaphore\n");
        
        for(int i = 0; i < 2; i++) {
            printf("*** Parent process working %d\n", i);
        }
        up(sem_id);
        printf("*** Parent process released semaphore\n");

    } else {
        printf("*** Fork failed\n");
        return 1;
    }

    printf("*** Semaphore tests done\n");
    printf("*** All tests done\n");
    
   

    shutdown();
    return 0;
}