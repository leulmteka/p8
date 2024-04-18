#include "libc.h"

void one(int fd) {
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));

    cp(fd,2);
}

// Test is intended to check if you can
// close stdin, stdout, and stderr
// since they're just initialized to that. Nothing states
// you can't close them.

// also tests fork I guess. I dunno
int main(int argc, char** argv) {
    printf("*** %d\n",argc);
    for (int i=0; i<argc; i++) {
        printf("*** %s\n",argv[i]);
    }
    int fd1 = open("/guh/bee.txt",0);
    one(fd1);

    printf("*** close = %d\n",close(fd1));
    one(fd1);

    // why does this even take ints in the first place
    // bad legacy decision, whoever is responsible for this.
    one(-1);

    printf("*** open again %d\n",open("/guh/bee.txt",0));
    printf("*** seek %ld\n",seek(3,260));
    
    int id1 = fork();

    if (id1 < 0) {
        printf("fork failed");
    } else if (id1 == 0) {
        /* child */
        printf("*** in child\n");
        int rc = execl("/sbin/check","in",0);
        printf("*** execl failed, rc = %d\n",rc);
    } else {
        /* parent */
        uint32_t status = 42;
        wait(id1,&status);
        printf("*** back from wait %ld\n",status);
    }

    shutdown();
    return 0;
}
