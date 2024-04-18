#include "libc.h"
#include "sys.h"

void one(int fd) {
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));

    cp(fd,2);
}

int main(int argc, char** argv) {
    printf("*** %d\n",argc); for (int i=0; i<argc; i++) {
        printf("*** %s\n",argv[i]);
    }
    int fd = open("/data/fortunes",0);
    one(fd);
    cp(fd, 1);
    
    printf("*** close = %d\n",close(fd));
    
    open("/etc/data.txt",0);

    for (int i = 0; i < 5; i++) {
        int id = fork();
        if (id < 0) {
            printf("*** fork failed on: %d", i);
            break;
        } else if (id > 0) {
            /* parent */
            uint32_t status = 42;
            wait(id,&status);
            printf("*** %d: back from wait %ld\n",i, status);
        } else if (id == 0) {
            printf("*** in child %d\n", i);
            int rc = execl("/sbin/shell","shell","a","b","c",0);
            printf("*** execl failed, rc = %d\n",rc);
        }
    }

    shutdown();
    return 0;
}
