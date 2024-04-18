#include "libc.h"

void one(int fd)
{
    printf("*** fd = %d\n", fd);
    printf("*** len = %d\n", len(fd));

    cp(fd, 2);
}

int main(int argc, char **argv)
{
    printf("*** %d\n", argc);
    printf("***I hope you didnt leave argvv pointing to something on the kernel\n");
    if ((uint32_t)(argv) < 0x00600000)
    {
        printf("***argv is pointing to something in the kernel. I'm going to over write it\n");
        printf("*** %lx\n", (uint32_t)(argv));
        uint32_t ptr = (uint32_t)(argv);
        while (ptr < 0x00600000)
        {
            *(uint32_t *)(ptr) = 0;
            ptr++;
        }
    }
    printf("***I couldn't access the kernel D:\n");

    for (int i = 0; i < argc; i++)
    {
        printf("*** %s\n", argv[i]);
    }
    printf("*** can we read the file we just opened ? \n");
    int fd = open("/etc/panic.txt", 0);
    one(fd);

    printf("***Im going to try to read into the kernel !!!\n");
    int success = read(fd, (uint32_t *)0x00001000, len(fd));
    if (success >= 0)
    {
        printf("*** BOOM KERNEL GONE\n");
        shutdown();
    }
    printf("*** kernel is safe >:(\n");
    // printf("*** close = %d\n", close(fd));

    printf("*** open again %d\n", open("/etc/panic.txt", 0));

    int id = fork();

    if (id < 0)
    {
        printf("fork failed");
    }
    else if (id == 0)
    {
        /* child */
        printf("*** in child\n");
        int rc = execl("/sbin/shell", "shell", "a", "b", "c", 0);
        printf("*** execl failed, rc = %d\n", rc);
    }
    else
    {
        /* parent */
        uint32_t status = 42;
        wait(id, &status);
        printf("*** back from wait %ld\n", status);

        int fd = open("/etc/panic.txt", 0);
        cp(fd, 1);
    }

    shutdown();
    return 0;
}
