#include "libc.h"

/**
 * Welcome to the Land of Failure! This test case, while somewhat functional, plans to break
 * manipulate, and exploit any oversight in your program (except symlinks because there's too many test
 * cases with those already and I don't feel like it!)
 * 
 * This will test correct memory addressing and managing errors involving these addresses. However,
 * there are also tests that involve kernel and APIC arguments alongside exploiting some oversight in the ELF
 * code that Gheith provided.
 * 
 * Without further ado, let's begin! (main method at the bottom, helper methods are in chronological order).
*/


/* These test the functionality and error handling of your open test case!*/
int open_tests() {
    printf("*** ====== Open Test Cases: ======\n");
    int fd1 = open("/etc/open_read.txt", 0);
    if (fd1 < 0) {
        shutdown();
    }
    printf("*** Basic Open Passed\n");

    // Make sure your code accounts for a non-existent file (should return a negative since file will be null) !
    int fd2 = open("/etc/does_not_exist.txt", 0);
    
    if (fd2 >= 0) {
        shutdown();
    }

    printf("*** Open Invalid File Passed\n");

    int fd3 = open("/etc/open_read.txt", 0);
    if (fd3 < 0) {
        shutdown();
    }

    printf("*** Open Same File Passed\n"); 

    int length = len(fd1);  

    close(fd1);
    close(fd3);
    if (length < 0) {
        shutdown();
    }
    printf("*** Closing Files Passed\n");
    
    return length;
}

/*  
 *  These tests check for every single method that can take an id in to manipulate a value (besides write, you'll see that later)
 *  Each failure is documented as all the values are printed out for the user to see in the raw data.
 *  
 *  These tests also make sure console access is limited to writes, any attempt to read or seek should fail.
 */
void invalid_id_tests() {
    printf("*** ====== Invalid ID Test Cases: ======\n");
    
    int length = len(-1);
    int up_check = up(-1);
    int down_check = down(-1);
    int close_check = close(-1);
    char buf[100];
    int read_check = read(-1, buf, 100);
    uint32_t status = 100;
    int child_check = wait(-1, &status);

    if (length >= 0 || up_check >= 0 || down_check >= 0 || close_check >= 0 || read_check >= 0) {
        shutdown();
    }

    printf("len: %d, up: %d, down: %d, close: %d, read: %d, child: %d\n", length, up_check, down_check, close_check, read_check, child_check);
    printf("*** Successfully failed Non-existent ID Tests\n");

    int read_from_console = read(1, buf, 100);
    int seek_from_console = seek(1, 100);

    // Check your sys calls for read and seek, do you prevent console access? (fd = 0)

    if (read_from_console >= 0) {
        printf("*** READ FROM CONSOLE... ERROR!\n");
        shutdown();
    }
    if (seek_from_console >= 0) {
        printf("*** SEEK FROM CONSOLE... ERROR!\n");
        shutdown();
    }
    printf("*** Successfuly failed Console Tests\n");
}

/**
 * These are some of the harder tests. They will ensure that accesses to read and write for
 * kernel and APICS fail. Some people check for the kernel space but disregard any access to the
 * APICs. You have to ensure both because we don't want either overwritten!
*/

void user_space_tests() {

    printf("*** ====== Accessing Kernel Space Test Cases: ======\n");

    // Make sure you limit access in read and write!

    int fd = open("/data/troll.txt", 0);
    int ioAPIC_read = read(fd, (uint32_t*) 0xFEC00000, len(fd));
    if (ioAPIC_read >= 0) {
        printf("*** Where's my ioAPIC?\n");
        shutdown();
    }
    int localAPIC_read = read(fd, (uint32_t*) 0xFEE00000, len(fd));
    if (localAPIC_read >= 0) {
        printf("*** Where's my localAPIC?\n");
        shutdown();
    }
    int kernel_read = read(fd, (uint32_t*) 0x00001000, len(fd));
    if (kernel_read >= 0) {
        printf("*** Where's my kernel?\n");
        shutdown();
    }

    printf("*** Successfully failed Reading into Kernel & APICs\n");

    // These writes will fail anyways because of the nature of write, but just to be
    // safe you should ensure that it doesn't let any APICs or Kernel values through.

    int ioAPIC_write = write(0, (uint32_t*) 0xFEC00000, len(fd));
    if (ioAPIC_write >= 0) {
        printf("*** Where's my ioAPIC?\n");
        shutdown();
    }
    int localAPIC_write = write(0, (uint32_t*) 0xFEE00000, len(fd));
    if (localAPIC_write >= 0) {
        printf("*** Where's my localAPIC?\n");
        shutdown();
    }
    int kernel_write = write(0, (uint32_t*) 0x00001000, len(fd));
    if (kernel_write >= 0) {
        printf("*** Where's my kernel?\n");
        shutdown();
    }
    close(fd);
    printf("*** Successfully failed Writing into Kernel & APICs\n");
}

/**
 * This is a basic child fork test that eventually evolves into an execl test. There are some portions
 * that are straightforward but some parts take advantage of the ability to pass in pointers to the sys
 * call arguments. Checks semaphore functionality, wait functionality, and execl functionality internally.
*/

void child_test() {
    printf("*** ====== Fork Test Cases: ======\n");
    int semaphore = sem(0);
    int id = fork();
    if (id == 0) {
        // we're in the child, we should wait here until the parent is done with some of their work.
        down(semaphore);
        printf("*** Accessing Child Passed!\n");
        printf("*** Let's go somewhere... more private.\n");

        // VERY IMPORTANT - This is taking us to shell.c which has our execl tests!!! Go there for
        // more information.

        execl("/sbin/shell", "shell", "Time", "To", "Do", 0);
        printf("*** execl failed... Awkwardddd\n");
        shutdown();
    }
    if (id > 0) {
        uint32_t* fake_status = (uint32_t *) 0x00001000;
        
        // We have to try to see if you account for the invalid pointer again... right? Anywhere there
        // is a pointer there is a possibility for exploitation! Even in wait! We don't want the kernel
        // or APICs getting overwritten with a simple child status!

        printf("*** Here's a little throwback Wait Test\n");
        int check = wait(id, fake_status);
        if (check >= 0) {
            printf("*** Is my status in the Kernel?\n");
            shutdown();
        }

        printf("*** Successfully handled Kernel Wait Status\n");
        up(semaphore);
        uint32_t status = 100;
        wait(id, &status);

        // We're back from the child now checking it exited properly.

        if (status != 90210) {
            printf("*** Child execution failed.\n");
            shutdown();
        }
        printf("*** ====== Return to Fork Tests: ======\n");
        printf("*** Successfully returned from Child\n");
    }
    if (id < 0) {
       printf("fork failed");
    }
}

int main (int argc, char** argv) {
    int test_len = open_tests();
    if (test_len != 62) {
        shutdown();
    }
    printf("*** Open Length Passed\n");

    invalid_id_tests();
    user_space_tests();
    child_test();
    printf("*** ====== ALL TESTS PASSED ======\n");
    shutdown();
    return 0;
}