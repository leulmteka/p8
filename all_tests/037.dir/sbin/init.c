#include "libc.h"

/*
This test looks if:
1. you can open a symbolic link, correctly get its length, and print it out
2. your open and close works for a full file descriptor table
3. your implementation of semaphores can be used to synchronize processes
4. your execl can use a child call a basic function to read 
*/

int main(int argc, char** argv) {
    //test symbolic link
    int fd = open("/data/symbolic",-123124); //make sure flags are ignored
    printf("*** length of symbolic is : %d\n", len(fd));
    cp(fd, 2, 28);
    printf("*** 1\n");
    printf("\n\n\n");

    //testing open and close
    int temp[7];
    for(int i=0;i<7;i++){
        temp[i]= open("/data/symbolic", -123124);
    }
    for (int i = 0; i < 7; i++)
    {
        close(temp[i]);
    }
    //try opening something now- should have space since we closed everything
    fd = open("/data/panic.txt", 0);
    printf("*** length of file is : %d\n", len(fd));
    cp(fd, 2, len(fd)/5); //dont print all of it out, print some
    printf("\n*** 2\n");

    // Semaphores Test
    int sem_global = sem(0);
    int sem_id1 = sem(0);
    int sem_id2 = sem(0);
    int sem_id3 = sem(0);

    if (sem_id1 < 0 || sem_id2 < 0)
    {
        printf("*** Failed to create semaphores\n");
        return -1;
    }

    printf("*** Created semaphores with IDs: %d, %d\n", sem_id1, sem_id2);

    int child_id1 = fork();
    if (child_id1 < 0)
    {
        printf("*** Fork failed\n");
        return -1;
    }
    else if (child_id1 == 0)
    {
        // Child process 1
        down(sem_id1);
        printf("*** Child process 1 acquired semaphore 1 \n");
        printf("*** Child process 1 doing some work\n");
        for (int i = 0; i < 100; i++)
        {
            printf("*");
        }
        printf("\n");
        up(sem_global);
        up(sem_id2);
        return 1;
    }

    int child_id2 = fork();
    if (child_id2 < 0)
    {
        printf("*** Fork failed\n");
        return -1;
    }
    else if (child_id2 == 0)
    {
        // Child process 2
        down(sem_id2);
        printf("*** Child process 2 acquired semaphore 2\n");
        printf("*** Child process 2 doing some work\n");
        for (int i = 0; i < 100; i++)
        {
            printf("*");
        }
        printf("\n");
        up(sem_global);
        up(sem_id3);
        return 2;
    }

    int child_id3 = fork();
    if (child_id3 < 0)
    {
        printf("*** Fork failed\n");
        return -1;
    }
    else if (child_id3 == 0)
    {
        // Child process 3
        down(sem_id3);
        printf("*** Child process 3 acquired semaphore\n");
        printf("*** Child process 3 doing some work\n");
        for (int i = 0; i < 100; i++)
        {
            printf("*");
        }
        printf("\n");
        up(sem_global);
        return 3;
    }
    up(sem_id1); // child 1 goes first
    down(sem_global);
    down(sem_global);
    down(sem_global);
    printf("*** 3\n");


    //Test execl : call our shell to read data.txt
    uint32_t status = -1;
    int exec_child = fork();
    if (exec_child == 0)
    {
        int rc = execl("/sbin/shell", "shell","/data/data.txt", "20", 0);
        return rc;
    }
    else
    {
        printf("*** Parent waiting for exec with status: %d\n", (int)status);
        wait(exec_child, &status);
        printf("*** All child processes finished, final status: %d\n", (int)status);
        printf("*** 4\n");
    }
    shutdown();
    }
