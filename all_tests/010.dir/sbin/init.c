#include "libc.h"

/**
 * Test 013: Basic test for basic functionalities
 * 
 * Test 1: Handling a full PD
 *  - This should truly already be handled for you by Gheith's implementation
 * 
 * Test 2: Reading files
 *  - This is an easy test that just reads a couple files and uses seek to ensure
 *    the correct offset is being set
 * 
 * Test 3: Handling symlinks
 *  - Can you handle symlinks across different directories?
 * 
 * Test 4: Writing to files
 *  - We know you cannot write to our file system, but do you return the correct 
 *    value anyway?
 */

int main(int argc, char** argv) {
    printf("*** Welcome to my AWESOME test case!\n***\n");

    /**
     * Test 1: Let's try opening multiple files!
     */

    printf("*** Test 1: Do you correctly handle a full FD?\n***\n");

    int awesome[11];

    for (int i = 0; i < 11; i++) {
        awesome[i] = open("ozymandias.txt",0);
    }

    if (awesome[7] == -1 && awesome[8] == -1 && awesome[9] == -1 && awesome[10] == -1) {
        printf("*** Passed Test 1!\n***\n");
    } else {
        printf("*** Failed Test 1!\n***\n");
        shutdown();
    }

    /**
     * Test 2: Let's read some files!
     */

    printf("*** Test 2: Seeking and reading files...\n***\n");

    seek(awesome[0], 44);
    cp(awesome[0], 2);

    printf("\n***\n");

    seek(awesome[1], 307);
    cp(awesome[1], 2);

    printf("\n***\n");

    cp(awesome[10], 2);

    for (int i = 0; i < 11; i++) {
        close(awesome[i]);
    }

    printf("***\n");

    printf("*** Passed Test 2!\n");

    /**
     * Test 3: Are you opening the correct file?
     */

    printf("***\n*** Test 3: Are you opening the correct file?\n");

    int fortnite = fork();

    if (fortnite < 0) {
        printf("fork failed");
    } else if (fortnite == 0) {
        int file = open("/file", 0);
        printf("***\n*** ");
        cp(file, 2);
        printf("\n");
        close(file);
    }

    uint32_t status = 420;
    wait(fortnite,&status);
    printf("***\n*** Passed Test 3!\n");

    /**
     * Test 4: Can you write to files?
     */

    printf("***\n*** Test 4: Can you write to files?\n***\n");

    int fd = open("ozymandias.txt",0);
    char buf[100];

    if (write(fd, buf, 100) == -1) {
        printf("*** Passed Test 4!\n");
    } else {
        printf("*** Failed Test 4!\n");
    }

    shutdown();
    return 0;
}
