#include "libc.h"

int main()
{
    // test opening a file that doesnt exist
    int invalidFd = open("/nonexistent/file.txt", 0);
    if (invalidFd < 0)
    {
        printf("*** Error opening nonexistent file\n");
    }

    // test reading from an invalid file descriptor
    char buffer[100];
    ssize_t bytesRead = read(invalidFd, buffer, sizeof(buffer));
    if (bytesRead < 0)
    {
        printf("*** Error reading from invalid file descriptor\n");
    }

    // Test seeking beyond the end of a file should be allowed
    int validFd = open("/etc/data.txt", 0);
    if (validFd >= 0)
    {
        off_t offset = seek(validFd, 10000);
        if (offset < 0)
        {
            printf("*** Should not be an error if seeking beyond end of file\n");
        }
    }

    // test closing invalid file descriptor
    int closeResult = close(invalidFd);
    if (closeResult < 0)
    {
        printf("*** Error closing invalid file descriptor\n");
    }

    // test closing file descriptor twice
    int duplicateFd = open("/etc/data.txt", 0);
    if (duplicateFd >= 0)
    {
        close(duplicateFd);
        closeResult = close(duplicateFd);
        if (closeResult < 0)
        {
            printf("*** Error closing file descriptor twice\n");
        }
    }

    // test waiting for invalid id
    int invalidId = 9999;
    uint32_t status;
    int waitResult = wait(invalidId, &status);
    if (waitResult < 0)
    {
        printf("*** Error waiting for invalid process ID\n");
    }

    // Test executing a nonexistent program
    int execlResult = execl("/nonexistent/program", "arg1", "arg2", 0);
    if (execlResult < 0)
    {
        printf("*** Error executing nonexistent program\n");
    }

    // Test executing with too many arguments
    int argc = 100;
    const char *argv[argc];
    for (int i = 0; i < argc; ++i)
    {
        argv[i] = "arg";
    }
    execl("/bin/program", argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], 0);

    // Test shutting down the system
    shutdown();

    return 0;
}