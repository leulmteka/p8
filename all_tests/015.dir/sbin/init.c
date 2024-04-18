#include "libc.h"

/*
 * This test case tests forking and creating semaphores as well as the closing of semaphores and files
 * 
 * In this testcase, we create a grandchild process that must properly handle semaphores and synchronization.
 * 
 * At the end, we also check if you correctly handle the first three file directory entries when they are closed and
 * your policy for adding new file directory entries.
 * 
 * This testcase also makes sure you can only use arguments in the private address space for execl.
 */

int main(int argc, char** argv) {



    int sem1 = sem(0);
    int sem2 = sem(1);
    int sem3 = sem(0);

    if (sem1 < 0 || sem2 < 0 || sem3 < 0) {
        printf("Creating semaphores failed\n");
        return 1;
    }

    int child = fork();
    if (child < 0) {
        printf("Failed to fork\n");
        return 1;
    } else if (child == 0) {
        down(sem1);
        printf("*** In child process\n");
        int grandchild = fork();
        if (grandchild < 0) {
            printf("Failed to fork grandchild\n");
            return 1;
        } else if (grandchild == 0) {
            down(sem2);
            printf("*** In grandchild process\n");
            printf("*** Reading file in grandchild process\n");
            int data2 = open("cs429.txt", 0);
            cp(data2, 2);
            printf("*** Grandchild process done\n");
            up(sem3);
            exit(0);
        }
        printf("*** Reading file in child process\n");
        int data1 = open("cs429 2.txt", 0);
        
        cp(data1, 2);
        //Testing execl
        int rc = execl("/sbin/shell", (char*) 0x00002000,"a","b","c",0);
        if (rc < 0){
            printf("*** Execl correctly failed\n");
        }

        printf("*** Child process done\n");
        up(sem2);
        exit(0);
    } else if (child > 0){

    }


    printf("*** Starting parent process\n");
    printf("*** Parent process releasing semaphore 1\n");
    up(sem1);


    uint32_t status;
    down(sem2);
    down(sem3);
    wait(child, &status);
    printf("*** Parent done waiting for child\n");
    printf("*** Checking how you handle the 3 special file directory entries\n");
    close(0);
    close(2);
    int data = open("data/data.txt", 0);
    if (data < 0) {
        printf("*** fail\n");
        return -1;
    }
    char buffer[len(data) + 1];
    if(read(data, buffer, len(data)) != -1) {
        printf("%s", buffer);
        printf("*** Can use the old file directory entry of stdout and read from the new file\n");
        printf("*** Passed handling special file directory entries\n");
    }
    if (close(data) == 0){
        printf("*** Successfully closed file\n");
    }

    if (close(sem1) == 0){
        printf("*** Successfully closed semaphore\n");
    }


    
    printf("*** Parent process finished\n");


    printf("*** SidChat is proud of you for handling forks and semaphores\n");

    shutdown();
    return 0;
}