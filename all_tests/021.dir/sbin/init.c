#include "libc.h"

/*****
 *
 * ========================================================== *
 *                  WELCOME TO MY TEST SUITE
 * ========================================================== *
 *
 *****/

/*
    Here is what the tests do:
    1. Tests how you handle offsets when the same file is opened over and over again (should start at the beginning offset)
    2. Writing a file (should return -1)
    3. Trying to excel a .txt file (should return -1 and not clear the private address space)
    4. Closing Files
    5. Opening Files

    There are commetned out print statements to help with debugging as well as checks
    This test took about 1.4 seconds when run on the lab machines
*/

int main(int argc, char **argv)
{

    printf("*** Welcome to the test case! \n");
    printf("*** Test 1: Opening the same file many times\n");
    int fd = open("favorites.dir/cardigan.txt", 0);

    for (uint32_t i = 0; i < 4; i++)
    {
        seek(fd, i * 100);
        fd = open("favorites.dir/cardigan.txt", 0);

        printf("*** fd = %d\n", fd);
    }
    // should print out the title of the song
    char buf[25];
    read(fd, buf, 25);
    printf("*** %s\n", buf);

    printf("*** fd = %d\n", fd);
    printf("*** len = %d\n", len(fd));
    seek(fd, 1602);

    // should print the last three lines
    cp(fd, 1);
    printf("\n");

    printf("\n*** Test 2: Trying to write to a file\n");
    ssize_t size = write(fd, buf, 25);
    printf("*** Result from write: %d\n", size);

    printf("\n*** Test 3: Execl from a non-existent file \n");
    int ans = execl("favorites.dir/teenage dreams.txt", "a", "b", "c", "d", 0);

    printf("*** Result post execl: %d\n", ans);

    printf("\n*** Test 4: Closing files \n");
    // let's close stdin and stderr
    close(0);
    close(2);
    // let's close all the files we previously had open
    for (int i = 3; i < 8; i++)
    {
        close(i);
    }
    printf("*** completed closing \n");

    printf("\n*** Test 5: Opening a file to stdin previous location \n");

    fd = open("favorites.dir/infinitely ordinary.txt", 0);
    char buf2[34];
    read(fd, buf2, 34);
    printf("*** %s\n", buf2);
    printf("*** fd = %d\n", fd);

    printf("\n*** Test 5 cont: in fork \n");
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
        uint32_t status = 101;
        wait(id, &status);
        printf("*** back from wait (also the age in teenage dream...) %ld\n", status);

        printf("\n*** karma is a cat, purring in my lap because it loves me!! congrats, you passed all the tests!!\n");
        printf("*** /\\_/\\\n");
        printf("***( o.o )\n");
        printf("*** > ^ <\n");
    }

    //  printf("*** %d\n", argc); // calling the write system call
    //  for (int i = 0; i < argc; i++)
    //  {
    //      printf("*** %s\n", argv[i]);
    //  }
    //  int fd = open("/etc/data.txt", 0);
    //  one(fd);

    // printf("*** close = %d\n", close(fd));

    // one(fd);
    // one(100);

    // printf("*** open again %d\n", open("/etc/data.txt", 0));
    // printf("*** seek %ld\n", seek(3, 17));

    // int id = fork();

    // if (id < 0)
    // {
    //     printf("fork failed");
    // }
    // else if (id == 0)
    // {
    //     /* child */
    //     printf("*** in child\n");
    //     int rc = execl("/sbin/shell", "shell", "a", "b", "c", 0);
    //     printf("*** execl failed, rc = %d\n", rc);
    // }
    // else
    // {
    //     /* parent */
    //     uint32_t status = 42;
    //     wait(id, &status);
    //     printf("*** back from wait %ld\n", status);

    //     int fd = open("/etc/panic.txt", 0);
    //     cp(fd, 1);
    // }

    shutdown();
    return 0;
}
