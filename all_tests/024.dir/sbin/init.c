#include "libc.h"

/* This test case builds on the t0 test case.

It tests the following: 
- trying to access a closed file
- opening a non-existent file
- trying to perform semaphore operations after closing it
- seek to an invalid position beyond the end of the file
- reading from this invalid position

*/

int main() {

    // Open a file
    int fd = open("/etc/data.txt",0);
    printf("*** File descriptor for data.txt: %d\n", fd);

    cp(fd,2);

    // Close the file
    printf("*** close = %d\n", close(fd));

    // Try to access the closed file
    printf("*** Trying to access closed file, fd = %d\n",fd);

    cp(fd,2);

    // Open a non-existent file (should return -1)
    printf("*** Opening non-existent file, fd = %d\n",25);

    cp(25,2);

    // Re-open the file
    printf("*** open again %d\n", open("/etc/data.txt",0));

    // Seek to a specific position in the file
    printf("*** seek %ld\n",seek(3,17));

    // Create a semaphore with initial value 1
    int sem_id = sem(1);
    printf("*** Semaphore created with ID: %d\n", sem_id);

    // Perform semaphore down operation
    printf("*** Semaphore down result: %d\n", down(sem_id));

    // Try to access the closed file again
    printf("*** Trying to access closed file, fd = %d\n",fd);

    cp(fd,2);

    // Close the semaphore
    printf("*** Semaphore close result: %d\n", close(sem_id));

    // Try to perform semaphore down operation after closing it
    printf("*** Semaphore down result after close: %d\n", down(sem_id));

    // Try to perform semaphore up operation after closing it
    printf("*** Semaphore up result after close: %d\n", up(sem_id));

    // Open a file
    int fd2 = open("/etc/data.txt", 0);
    printf("*** File descriptor for data.txt: %d\n", fd2);

    // Get the length of the file
    ssize_t file_length = len(fd2);
    printf("*** Length of the file: %d\n", file_length);

    // Seek to an invalid position beyond the end of the file
    off_t new_offset = seek(fd2, file_length + 100);
    printf("*** New offset after seeking: %ld\n", new_offset);

    // Attempt to read from the file
    char buffer[100];
    ssize_t bytes_read = read(fd2, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        printf("*** Read error\n");
    } else {
        printf("*** Bytes read: %d\n", bytes_read);
    }

    // Close the file
    printf("*** Close result: %d\n", close(fd2));
 

    shutdown();
    return 0;
}
