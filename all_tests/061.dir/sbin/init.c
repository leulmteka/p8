#include "libc.h"

void one(int fd)
{
    printf("*** fd = %d\n", fd);
    printf("*** len = %d\n", len(fd));

    cp(fd, 2);
}

int main(int argc, char **argv)
{
    printf("*** This is my test! :D \n");

    // lets try opening a bunch of files
    // makes sure no issues are in how your files are linked between multiples of the same file
    int fd[10];
    for(int i = 0; i < 10; i++){
        fd[i] = open("/etc/data.txt", 0);
    }
    seek(fd[9], 100);
    one(fd[0]); 
    close(fd[5]);
    one(fd[4]);
    one(fd[9]);
    one(fd[5]);

    
    int id = fork();

        if (id < 0) {
            printf("fork failed");
        } else if (id == 0) {
            /* child */
            printf("*** Hi i'm child\n");
            one(fd[6]);
            return 0;
        } else {
        /* parent */
        uint32_t status = 0;
        wait(id,&status);
        printf("*** back in parent \n");
        }
    one(fd[6]);

    shutdown();
    return 0;
}
