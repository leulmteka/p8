#include "libc.h"

int main(int argc, char** argv) {
    printf("*** %d\n",argc);
    for (int i=0; i<argc; i++) {
        printf("*** %s\n",argv[i]);
    }
    int fd = open("/etc/data.txt",0);
    len(fd);

    // if you fail here make sure u cant close and already closed file
    close(fd);
    if(close(fd) >= 0){
        printf("close is incorrect incorectly handles consecutive closes\n");
    }

    // make sure you return a negative value if the file doesnt exsit
    if(open("/huzzag.txt", 0) < 0){
        printf("*** correctly handels noexistent files\n");
    }


    open("/etc/data.txt",0);

    // seeking past the file isn't an error!
    if(seek(3,100000) >= 0){
        printf("*** seeking larger than filesize didnt create an error!\n");
    }
    

    shutdown();
    return 0;
}
