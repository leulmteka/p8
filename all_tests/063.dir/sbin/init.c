#include "libc.h"

void one(int fd) {
    printf("*** fd = %d\n",fd);
    // tests len() system call
    printf("*** len = %d\n",len(fd));

    cp(fd,1);
}

// this testcase tests the functionality of system calls by 
// reading file contents and creating child threads from
// the parent thread
int main(int argc, char** argv) {
    for (int i=0; i<argc; i++) {
        printf("*** %s\n",argv[i]);
    }
    int welcome_file = open("/startingfolder/welcome.txt",0);
    one(welcome_file);

    printf("\n*** close = %d\n",close(welcome_file));
    printf("*** checking if file closed successfully\n");
    one(welcome_file);
    printf("*** check passed\n");

    printf("*** attempting to open again %d\n",open("/startingfolder/welcome.txt",0));
    printf("*** testing seek %ld\n",seek(welcome_file, 12));

    int other_file = open("/otherfolder/jake.txt", 0);
    one(other_file);
    printf("\n*** seek other file %ld\n",seek(other_file, 7));
    
    int fork_id = fork();

    if (fork_id < 0) {
        printf("*** fork failed");
    } else if (fork_id == 0) {
        /* child */
        int rc = execl("/sbin/shell","this", "is", "a", "good", "testcase", 0);
        printf("*** execl failed, rc = %d\n",rc);
    } else {
        /* parent */
        uint32_t status = 96;
        wait(fork_id,&status);
        int vampire = open("/startingfolder/vampire.txt",0);
        printf("*** vampire fd number is %d\n", vampire);
        cp(vampire,1);
    }
    printf("\n");
    shutdown();
    return 0;
}
