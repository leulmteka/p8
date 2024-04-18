#include "libc.h"

/* 
    This test has a very similar structure to the t0, which is where I first duplicated from. 
    This test does take into account a few additional things, particulary with passing in 
    kernel addresses into system calls. Semaphore related explicit system calls are not being
    tested.
 */

void one(int fd) {
    printf("*** len = %d\n",len(fd));

    cp(fd,2);
}

int main(int argc, char** argv) {
    // testing initial exec; very standard
    printf("*** %d\n",argc);
    for (int i=0; i<argc; i++) {
        printf("*** %s\n",argv[i]);
    }

    // testing if you are able to handle non-existent file
    int fd = open("/etc/not_exist.txt",0);
    if (fd != -1) {
        printf("*** Invalid file, should not open");
        shutdown();
    }
    // testing if you are able to handle kernel corruption for open
    fd = open((char*) (0x00010000),0);
    if (fd != -1) {
        printf("*** Invalid address, should not open");
        shutdown();
    }

    // finally opening a real file
    fd = open("/etc/butter.txt",0);
    if (fd != 3) {
        printf("*** Not honoring first open space");
        shutdown();
    }
    one(fd);

    printf("*** close = %d\n",close(fd));


    printf("*** attempting to open butter again %d\n",open("/etc/butter.txt",0));
    printf("*** attempting to close butter again %d\n",close(3));
    
    int id = fork();

    if (id < 0) {
        printf("fork failed");
    } else if (id == 0) {
        /* child */
        printf("*** in child\n");
        // passing in kernel address to execl, operation can not complete
        int rc = execl("/sbin/shell","shell","a",(char*)(0x00003000),"c",0);
        printf("*** you should still be here!\n");

        rc = execl("/sbin/shell","shell","a","b","c",0);
        printf("*** execl failed, rc = %d\n",rc);
    } else {
        /* parent */
        uint32_t status = 42;
        wait(id,&status);
        printf("*** back from wait %ld\n",status);

        fd = open("/etc/panic.txt",0);
        cp(fd,1);
    }

    // seek file you have not opened
    int not_opened = seek(0x1000005, 100);
    if (not_opened != -1) {
        printf("*** how can you seek a file you have not opened\n");
    }
    
    int finale = open("/etc/finale.txt", 0);
    one(finale);

    shutdown();
    return 0;
}

