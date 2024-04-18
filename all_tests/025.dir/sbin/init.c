#include "libc.h"

void seekTest() {
    /*
    offsets[] is an array of offset values, each specifying one char in seek.txt. Together, it should spell out "FILE SYSTEMS!"
    If you're not getting the right chars, your seek isn't properly updating the offset value, or your read isn't reading from the offset value
    This took me forever so appreciate it pls :)
    */

    int fd = open("data/seek.txt", 0);
    uint32_t offsets[] = {719, 729, 794, 801, 826, 912, 929, 1054, 1082, 1168, 1269, 1393, 1531};

    printf("*** ");
    for (int i = 0; i < 13; i++) {
        char buf[1];
        seek(fd, offsets[i]);
        read(fd, buf, 1);
        printf("%s", buf);
    }
    printf("\n");

    close(fd);
}

void recursiveFork(int level, int sem_id) {
    /*
    Helper method for forkTest, read comment in forkTest for details on how it's supposed to work
    */

    int child = fork();
    if (child < 0) {
        printf("*** Fork %d failed rip\n", level);
        shutdown();
    }
    if (child == 0) {
        recursiveFork(level - 1, sem_id);
    } else if (child > 0) {
        down(sem_id);
        printf("*** Fork %d successful yay\n", level);
        up(sem_id);
        if (level <= 1) return;
        uint32_t status;
        wait(child, &status);
        exit(0);
    }
}

void forkTest() {
    /*
    Throwback to recursion in data structures #mynightmare this method recursively tests your forking
    It should create 4 children which print out in decrementing order (fork 4 to fork 1)
    */

    int sem_id = sem(1);
    recursiveFork(4, sem_id);
    printf("*** Parent goes last\n");
}

void idAllocationTest() {
    /*
    When opening files, you should assign it to the first available file descriptor value
    If you're shutting down on any part of this test, either your close() isn't properly nulling the value at index fd in the fd table
    or you're not checking for lowest available fd value
    */

   char* one = "data/one.txt";
   char* two = "data/two.txt";
   char* three = "data/three.txt";
   char* four = "data/four.txt";
   char* five = "data/five.txt";

   int fd1 = open(one, 0);
   if (fd1 != 3) shutdown();
   char buf1[12];
   read(fd1, buf1, 12);
   printf("%s\n", buf1);
   
   int fd2 = open(two, 0);
   if (fd2 != 4) shutdown();

   int fd3 = open(three, 0);
   if (fd3 != 5) shutdown();

   int fd4 = open(two, 0);
   if (fd4 != 6) shutdown();
   char buf2[12];
   read(fd4, buf2, 12);
   printf("%s\n", buf2);

   close(fd1);

   int fd5 = open(one, 0);
   if (fd5 != 3) shutdown();

   close(fd3);

   int fd6 = open(four, 0);
   if (fd6 != 5) shutdown();

   close(fd2);

   close(fd4);

   int fd7 = open(one, 0);
   if (fd7 != 4) shutdown();

   int fd8 = open(three, 0);
   if (fd8 != 6) shutdown();
   char buf3[12];
   read(fd8, buf3, 12);
   printf("%s\n", buf3);
   
   int fd9 = open(two, 0);
   if (fd9 != 7) shutdown();

   int fd10 = open(three, 0);
   if (fd10 != 8) shutdown();

   close(fd9);

   int fd11= open(four, 0);
   if (fd11 != 7) shutdown();
   char buf4[13];
   read(fd11, buf4, 13);
   printf("%s\n", buf4);

   int fd12 = open(five, 0);
   if (fd12 != 9) shutdown();
   char buf5[21];
   read(fd12, buf5, 21);
   printf("%s\n", buf5);
}

int main(int argc, char** argv) {
    /*
    Each testing helper method has a description of what they do and where you could go wrong if you're failing any part
    Overall this tests your seek (if you're updating offset right and reading from the right offset).
                            id allocation (if you're allocating the lowest possible fd and closing properly),
                            and fork (can you revursively fork and exist from each child properly?)
    */

    printf("*** Testing seek!\n");
    seekTest();

    printf("*** Testing id allocation!\n");
    idAllocationTest();

    printf("*** Testing fork!\n");
    forkTest();

    printf("*** Done!\n");

    shutdown();
    return 0;
}
