#include "libc.h"
#include "sys.h"

// This is a general test for over 10 recursive symlinks in open
// Also make sure your semaphore methods work 


int main(int argc, char** argv) {
    

    printf("*** Testing 10 recursive symlink\n");
    int final_boss = open("/8", 0);
    if (final_boss < 0) {
        printf("*** fail\n");
        return -1;
    }

    printf("*** Success\n");

    printf("*** Testing semaphore\n");
    int s = sem(1);
    
    if(s < 0){
        printf("*** fail\n");
        return -1;
    }

    int id = fork();

    if (id < 0) {
        printf("fork failed");
    }

    down(s);
    char buf[1];
    read(final_boss, buf, 1);
    if (buf[0] == '1') {
        printf("*** Success\n");
    } else if (buf[0] == '0') {
        shutdown();
        printf("*** fail\n");
    }
    up(s);

    return 0;

}