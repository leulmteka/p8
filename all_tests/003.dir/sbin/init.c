#include "libc.h"

void one(int fd) { // FROM t0
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));

    // cp(fd,2);
}

int main(int argc, char** argv) {
    printf("*** Test Start\n");
    
    
    /* Test whether opening a file that doesn't exist fails */
    int invld = open("/data/not_here.txt", 100);
    if (invld >= 0) {
        printf("-----Check your open, you have 'found' a file that does not exist!\n");
    } else {
        printf("*** Correctly could not find a file that doesn't exist!\n");
    }

    /* Tests that open correctly finds a file that exists and that seeking past the size of file doesnt return an error */
    int fd = open("/data/data.txt",0);
    if (fd < 0) {
        printf("-----Check your open, you did not find a file that exists!\n");
        
    } else {
        // one(fd);

        off_t sk = seek(fd, 123456);
        if (sk >= 0) {
            printf("*** Correctly did not cause an error from seeking past size of file!\n");
        } else {
            /* According to sys.h in t0:
            seeking outside the file is not an error but might cause
            subsequent read/write to fail 
            */
            printf("-----Check your seek, you should not cause an error from seeking past size of file!\n"); 

        }
    }

    /* Tests that a double close causes an error */
    int cls = close(fd);
    cls = close(fd);
    if (cls >= 0) {
        printf("-----Check your close, you should not be returning a non-neg value from a double-close!\n");
    } else {
        printf("*** Correctly handled double closes!\n");
    }

    /* Tests that execl on a file that doesn't exist causes an error */
    int exc = execl("/data/not_here.txt", "lol", "-100", "100", 0);
    if (exc >= 0) {
        printf("-----Check your exec/execl, you returned a non-neg value from executing a file that doesn't exist!\n");
    } else {
        printf("*** Correctly failed to execute a nonexistent file!\n");
    }

    /* Tests that an ELF header entry exists only within the private user space 0x80000000 - 0xFFFFFFFF+1 */
    // Assumption the upper bound is inclusive, [0x80000000, 0xFFFFFFFF+1] and not [0x80000000, 0xFFFFFFFF+1)
    

    shutdown(); // Do i need to call shutdown manually? Idk t0 does it, so i will
    return 0;
}
