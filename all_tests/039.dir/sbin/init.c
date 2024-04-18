#include "libc.h"

// print out file directory, length, and reads file
void one(int fd) {
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));

    cp(fd,2);
}

int main(int argc, char** argv) {
    // Checks argc is correct value
    printf("*** %d\n",argc);

    // Checks argv is at the correct position on the stack, correct values
    for (int i=0; i<argc; i++) {
        printf("*** %s\n",argv[i]);
    }

    // testing open, contents of data
    int fd = open("/etc/magnetic.txt",0);
    //ASSERT(fd == 3);
    one(fd);

    printf("*** close = %d\n",close(fd));

    one(fd);
    one(100);

    // try opening magnetic again, testing seek
    printf("*** open again %d\n",open("/etc/magnetic.txt",0));
    printf("*** seek %ld\n",seek(3,17));
    
    int id = fork();

    if (id < 0) {
        printf("fork failed");
    } else if (id == 0) {
        /* currently in the child process */
        printf("*** in child\n");
        int rc = execl("/sbin/shell","shell","a","b","c",0);
        printf("*** execl failed, rc = %d\n",rc);
    } else {
        /* currently in the parent process */
        uint32_t status = 42;
        wait(id,&status);
        printf("*** back from wait %ld\n",status);

        int fd = open("/etc/riri.txt",0);
        cp(fd,1);
    }

    shutdown();
    return 0;
}
