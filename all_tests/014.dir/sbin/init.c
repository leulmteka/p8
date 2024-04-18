#include "libc.h"

/**
 * My test tests opening, closing, and length checking implementation on two different files
*/
void testFileOperations(const char* filePath) {
    printf("*** Testing file operations for: %s\n", filePath);
    
    // Open the file
    int fd = open(filePath, 0); // Assuming 0 specifies read mode
    if (fd < 0) {
        printf("*** Failed to open file: %s\n", filePath);
        return; // Exit this function if file cannot be opened
    }
    printf("*** File opened successfully with fd = %d\n", fd);

    // Print the file length
    printf("*** Length of the file: %d\n", len(fd));

    // Close the file
    int closeResult = close(fd);
    printf("*** File close operation returned: %d\n", closeResult);
}

int main(int argc, char** argv) {
    printf("*** Number of arguments: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("*** Argument %d: %s\n", i, argv[i]);
    }

    // Test opening, closing, and length checking 
    testFileOperations("/etc/lastChristmas.txt");

    
    testFileOperations("/etc/tree.txt");

    shutdown();
    return 0;
}
