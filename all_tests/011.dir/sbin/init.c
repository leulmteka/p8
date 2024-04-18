#include "libc.h"


/* 
This test case tests if your implementation correctly 
handles reads and writes from / to kernel memory and 
writes to the LAPIC / IOPIC 
*/
int main(int argc, char** argv) {

    
    printf("*** %d\n",argc);
    for (int i=0; i<argc; i++) {
        printf("*** %s\n",argv[i]);
    }

    uint32_t bad_address = 0x8000; 
    int fd = open("/etc/data.txt",0);

    /* Test if read rejects buf in kernel space */
    ssize_t ret = read(fd, (void*) bad_address, 5); 
    if (ret != -1) {
        return -1;
    }
    printf("*** made it past kernel space read test\n");

    /* Test if write rejects buf in kernel space */
    ret = write(fd, (void*) bad_address, 5);
    if (ret != -1) {
        return -1;
    }
    printf("*** made it past kernel space write test\n");

    /* Test if write rejects buf located in LAPIC */
    bad_address = 0xfee00000;
    ret = write(fd, (void*) bad_address, 5);
    if (ret != -1) {
        return -1;
    }
    printf("*** made it past write to LAPIC test\n");

    /* Test if write rejects buf located in IOPIC */
    bad_address = 0xfec00000;
    ret = write(fd, (void*) bad_address, 5);
    if (ret != -1) {
        return -1;
    }
    printf("*** made it past write to IOPIC test\n");

    shutdown();
    return 0;
}
