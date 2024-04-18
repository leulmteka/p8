#include "libc.h"

int main(int argc, char **argv)
{

    int fd = open("/data/panic.txt", 0);
    cp(fd, 1); // dump to stdout

    if (close(fd) != 0)
    {
        printf("*** Failed to close file.\n");
        return -1;
    }

    int fd1 = open("/data/panic.txt", 0);
    int fd2 = open("/data/panic.txt", 0);
    if (fd1 == fd2)
    {
        printf("*** Not creating unique file descriptors for the same file.\n");
        return -1;
    }
    int seek1 = seek(fd1, 10);
    int seek2 = seek(fd2, 5);
    if (seek1 != seek2 * 2)
    {
        printf("*** Your seek return value is incorrect.\n");
        return -1;
    }

    int len1 = len(fd1);
    int len2 = len(fd2);

    if (len1 != len2)
    {
        printf("*** Your len return value is incorrect.\n");
    }

    int child_ids[3];

    for (int i = 0; i < 3; i++)
    {
        child_ids[i] = fork();

        if (child_ids[i] < 0)
        {
            printf("*** Fork failed.\n");
            return -1;
        }
        else if (child_ids[i] == 0)
        {
            // We are in a child process
            char c = '0' + i + 1;
            char param[2];
            param[0] = c;
            param[1] = '\0';
            execl("/sbin/program", "program", param, "foo", "bar", "baz", "qux", 0);
            printf("*** SHOULD NOT RETURN, CHECK YOUR EXEC/execL FUNCTIONS.\n");
            return -1;
        }
        // We are the parent process, continue to make another child
    }

    for (int i = 0; i < 3; i++)
    {
        uint32_t status = 0;
        wait(child_ids[i], &status);
        if ((status) != 777)
        {
            printf("*** Wrong return value for wait.\n");
            return -1;
        }
    }

    printf("*** I'm a good parent, I waited on all my children.\n");
    shutdown();
    return 0;
}
