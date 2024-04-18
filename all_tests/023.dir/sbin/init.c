#include "libc.h"

/**
 * This test case is designed to test if there are any errors within your program
 * and will try to see if you have protections against inputs that are meant
 * to break your code
*/

/**
 * This tests your open method, seek method, and your 
 * ability to handle reading symlinks recusively
*/
void open_symlink_test() {
    printf("*** Testing Open and Seek\n");
    int code = open("/Treasure", 0);
    if (code < 0) {
        printf("*** Failed Open\n");
        shutdown();
    }
    char buffer[1];
    seek(code,7);
    printf("*** ");
    cp(code,1);

    printf("\n***\n*** Open Non-existent Files and Reopening File Test\n");
    int file1 = open("/non_existent_file", 0);
    if (file1 >= 0) {
        printf("*** Fail Opening Non-existent File\n");
        shutdown();
    }

    printf("***\n*** Open Invalid File Test\n");
    int file2 = open("/Treasure", 0);
    if (file2 < 0) {
        printf("*** Fail Reopening File\n");
        shutdown();
    }

    printf("***\n*** Testing Symlink\n");
    int symlink = open("/This", 0);
    if (symlink < 0) {
        printf("*** Failed Open\n");
        shutdown();
    }

    read(symlink, buffer, 1);
    if (buffer[0] == 'R') {
        printf("*** Passed Open and Symlink Tests!\n");
    } else {
        printf("*** Failed Recursive Symlink Test\n");
        shutdown();
    }
}

/*  
 *  Checks if your methods successfully fails invalid inputs
 */
void invalid_input_test() {
    printf("***\n*** Invalid Input Tests\n");
    
    char buffer[1];
    uint32_t stat = 1;

    if (len(-1) >= 0 || up(-1) >= 0 || down(-1) >= 0 || 
        close(-1) >= 0 || read(-1, buffer, 1) >= 0 || wait(-1, &stat) >= 0) {
        printf("*** Failed Invalid Input Tests\n");
        shutdown();
    }

    printf("*** Successfully failed Non-existent ID Tests\n");
}

/**
 * This tests if accesses to the kernel space fails
*/
void kernel_access_tests() {
    printf("***\n*** Kernel Space Test\n");

    int test_fd = open("/Fun", 0);

    for (uint32_t addr = 0x00000000; addr < 0x00600000; addr += 0x1000) {
        seek(test_fd, 0);
        int result = read(test_fd, (uint32_t *) addr, 1);
        if (result != -1) shutdown();
    }

    printf("*** Successfully failed Reading to Kernel Space\n");
}

/**
 * This is a child fork test that also tests for your execl method.
*/
void fork_test() {
    printf("***\n*** Fork Test\n");
    int id = fork();

    if (id < 0) {
        printf("fork failed");
        shutdown();
    } else if (id == 0) {
        printf("*** in child\n");
        int rc = execl("/sbin/shell","Checking","Your","Execl",0);
        printf("*** execl failed, rc = %d\n",rc);
    } else {
        uint32_t status = 42;
        wait(id,&status);
        printf("*** back from wait %ld\n",status);
    }
}

int main (int argc, char** argv) {
    open_symlink_test();
    invalid_input_test();
    kernel_access_tests();
    fork_test();

    shutdown();
    return 0;
}