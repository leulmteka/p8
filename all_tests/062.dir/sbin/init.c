#include "libc.h"

/*void one(int fd) {
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));

    cp(fd,2);
}*/

int main(int argc, char** argv) {
printf("\n*** Begin\n");
    // Seek test.
    int fd = open("/data/data.txt", 0);
    if (fd < 0){
        printf("*** Failure\n");
    }else{
        off_t off = seek(fd, 1);
        if(off < 0){
            printf("*** Failure\n");
        }else{
            // Seek should not return an error when seeking outside the file.
            off_t off2 = seek(fd, 100000);
            if(off2 < 0){
                printf("*** Failure\n");
            }else{
                printf("*** Pass\n");
            }
        }
    }

    // Test close on a file and semaphore.
    int sem1 = sem(1);
    int close_val = close(sem1);

    if(close_val < 0){
        printf("*** Failure\n");
    }else{
        printf("*** Pass\n");
    }
    // Double close should be an error.
    if(close(sem1) >= 0){
        printf("*** Failure\n");
    }else{
        printf("*** Pass\n");
    }

    int close_val2 = close(fd);
    if(close_val2 < 0){
        printf("*** Failure\n");
    }
    // Double close should be an error.
    if(close(fd) >= 0){
        printf("*** Failure\n");
    }else{
        printf("*** Pass\n");
    }

    
    // Test open and execl on a path that does not exist.
    int test = open("/data/does_not_exist.txt", 0);
    if(test >= 0){
        printf("*** Failure\n");
    }else{
        printf("*** Pass\n");
    }

    int val = execl("/data/does_not_exist.txt", "DNE", 0);
    if(val >= 0){
        printf("*** Failure\n");
    }else{
        printf("*** Pass\n");
    }

    shutdown();
    return 0;
}
