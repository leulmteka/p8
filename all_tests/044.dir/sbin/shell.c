#include "libc.h"

/*

    shell.c is made to test file operations and correct execution of a child process independent from the parent.
    Acts as a child process that is created by the parent process (init.c) with the fork() method.

    Main Tests:
        1.) Child process can do file operations independent of the parent process
        2.) Ensures correct cleanup and termination

    If you're failing this test case, it may be due to the following reasons:
        1.) Improper implementation of path resolution if you cannot  open, close, or read

*/

int main(int argc, char** argv) {
    printf("*** Child Test Start\n");

    // Open hello and check if it exists
    int fd = open("/hello", 0);
    if (fd < 0) {
        printf("*** Failed to open file hello\n");
        return 1;
    }

    // Read and display the contents of hello
    char buffer[256];
    int bytes = read(fd, buffer, 255);
    buffer[bytes] = '\0';
    printf("%s\n", buffer);
    // Release the resource
    close(fd);

    printf("*** Child Test Complete\n");
    return 0;
}
