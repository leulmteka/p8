#include "libc.h"

int main(int argc, char** argv) {
    //First test, make sure we only hold 10 files 
    //knowing that we fd 0 1 and 2 are already taken, 
    //lets see if we can open 8 files.
    for(int i = 0; i < 7; i++){
        int fd = open("/files/smile.txt", 0);
        if(fd == -1){
            printf("*** Failed opening one of the first 7 files this should work and not fail\n");
        }
    }

    int fd = open("/files/smile.txt", 0);
    if(fd == -1){
        printf("*** Passed file test, only holding 10 file descriptors\n");
    } else {
        printf("*** Failed file test opened 8 files\n");
    }

    close(0);
    fd = open("/files/smile.txt", 0);

    if(fd == 0){
        printf("*** Passed open test, filling unused descriptor index\n");
    } else {
        printf("*** Failed open test new file should be in descriptor 1\n");
    }

    //Final test to determine if execl returns from a non executable file
    execl("/files/notanexecutable.txt", "notanexecutable", 0);
    printf("*** Properly returned from a faulty execl\n");

    execl("/sbin/goodexecutable", "goodexecutable", "439", 0);
    return 0;
}
