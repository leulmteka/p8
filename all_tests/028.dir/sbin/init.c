#include "libc.h"

/*
    Tests for edge case checks for system calls.
    System Calls tested are the following:
        open
        len
        write
        read
        up
        down
        close

    Could've tested more but was crunchtime.

    OOP.txt contains SPOILERS to JJK Manga: CAREFUL IF YOU DONT WANT TO BE SPOILED
        WILL BE PRINTED IF FAIL OR PASS.
*/

// from t0
void one(int fd)
{
    printf("*** fd = %d\n", fd);
    printf("*** len = %d\n", len(fd));

    cp(fd, 2);
}

int main(int argc, char **argv)
{

    printf("*** %d\n", argc);
    printf("*** %s\n", argv[0]);
    printf("*** WHO WINS GOJO OR SUKUNA >:).\n");

    /*

        LEMME DO SUM NULLPTR AND GIBBERISH CHECKS ON YOUR SYSCALLs, TO TEST IF THEY BREAK AND STUFFS>;DDDDDDDDDDDDDDDDD
    
    */

    /*
        Test open: nullptr buff, bad path, lost+found dir
    */ 
    // nullptr
    const char* bait = 0;
    int fd = open(bait, 0);
    if (fd != -1) {
        printf("Testing Open: failed taking in a nullptr! WE GOTTA END DIS NOW\n");
        goto failed;
    }
    printf("*** Testing Open: Passed taking in a nullptr! KEEP ON GOING TY SH\n");
        
    // lost+found dir
    fd = open("..", 0);
    if (fd != -1) {
        printf("Testing Open: failed taking in a dir! WE GOTTA END DIS NOW\n");
        goto failed;
    }
    printf("*** Testing Open: Passed taking in a dir! KEEP ON GOING TY SH\n");

    // bad path
    fd = open("/data/nah_id_win.txt", 0);
    if (fd != -1) {
        printf("Testing Open: failed taking in a bad path! WE GOTTA END DIS NOW\n");
        goto failed;
    }
    printf("*** Testing Open: Passed taking in a bad path! KEEP ON GOING TY SH\n");
    printf("*** \n");


    /*
        Test len: bad fd
    */
    // bad fd
    ssize_t length = len(-45678);
    if (length != -1) {
        printf("Testing Len: failed taking in a bad fd! WE GOTTA END DIS NOW\n");
        goto failed;
    }
    printf("*** Testing Len: Passed taking in a bad fd! KEEP ON GOING TY SH\n");
    printf("*** \n");


    /*
        Test write: nullptr buff, bad fd
        but tbh write should return -1, cause we don't got no write feature
    */
    // nullptr buff
    int tempfd = 3;
    char *nullbuf = 0;
    size_t tempbytes = 1234; 
    ssize_t wrote = write(tempfd, (void *) nullbuf, tempbytes);
    if (wrote != -1) {
        printf("Testing Write: failed taking in a nullptr buf! WE GOTTA END DIS NOW\n");
        goto failed;
    }
    printf("*** Testing Write: Passed taking in a nullptr buf! KEEP ON GOING TY SH\n");

    // bad fd
    tempfd = -1;
    char *tempbuf[4];
    wrote = write(tempfd, (void *) tempbuf, tempbytes);
    if (wrote != -1) {
        printf("Testing Write: failed taking in a bad fd! WE GOTTA END DIS NOW\n");
        goto failed;
    }
    printf("*** Testing Write: Passed taking in a bad fd! KEEP ON GOING TY SH\n");
    printf("*** \n");


    /*
        Test read: nBytes < 0, nullptr buff, bad fd, IOPIC, LAPIC
    */

    // nBytes < 0
    tempfd = 3;
    char *tempBuf[4];
    size_t tempBytes = -1234;
    ssize_t red = read(tempfd, (void *) tempBuf, tempBytes);
    if (red != -1) {
        printf("Testing Read: failed taking in negative tempBytes! WE GOTTA END DIS NOW\n");
        goto failed;
    }
    printf("*** Testing Read: Passed taking in negative tempBytes! KEEP ON GOING TY SH\n");

    // nullptr buff
    tempfd = 3;
    char *nullBuf = 0;
    tempBytes = 5;
    red = read(tempfd, nullBuf, tempBytes);
    if (red != -1) {
        printf("Testing Read: failed taking in null buffer! WE GOTTA END DIS NOW\n");
        goto failed;
    }
    printf("*** Testing Read: Passed taking in null buffer! KEEP ON GOING TY SH\n");

    // bad fd
    tempfd = -5;
    char *tempBuf2[5];
    tempBytes = 4;
    red = read(tempfd, (void *) tempBuf2, tempBytes);
    if (red != -1) {
        printf("Testing Read: failed taking in a bad fd! WE GOTTA END DIS NOW\n");
        goto failed;
    }
    printf("*** Testing Read: Passed taking in bad fd! KEEP ON GOING TY SH\n");

    // ioAPIC: fec00000
    tempfd = 3;
    uint32_t IOPIC = 0xfec00000;
    void *IOPIC_ptr = (uint32_t *) IOPIC;
    tempBytes = 4;
    red = read(tempfd, IOPIC_ptr, tempBytes);
    if (red != -1) {
        printf("Testing Read: failed not taking IOPIC as buffer ptr address! WE GOTTA END DIS NOW\n");
        goto failed;
    }
    printf("*** Testing Read: Passed not taking IOPIC as buffer ptr address! KEEP ON GOING TY SH\n");

    // ioAPIC: fee00000
    tempfd = 3;
    uint32_t LAPIC = 0xfee00000;
    void *LAPIC_ptr = (uint32_t *) LAPIC;
    tempBytes = 4;
    red = read(tempfd, LAPIC_ptr, tempBytes);
    if (red != -1) {
        printf("Testing Read: failed not taking LAPIC as buffer ptr address! WE GOTTA END DIS NOW\n");
        goto failed;
    }
    printf("*** Testing Read: Passed not taking LAPIC as buffer ptr address! KEEP ON GOING TY SH\n");
    printf("*** \n");


    /*
        Test up: bad id
    */
    // bad id
    int fakeID = -1234;
    int uppies = up(fakeID);
    if (uppies != -1) {
        printf("Testing Up: failed not taking bad sem ID! WE GOTTA END DIS NOW\n");
        goto failed;
    }
    printf("*** Testing Up: Passed not taking bad sem ID! WE GOTTA END DIS NOW\n");


    /*
        Test down: bad id
    */
    // bad id
    int downzies = down(fakeID);
    if (downzies != -1) {
        printf("Testing Down: failed not taking bad sem ID! WE GOTTA END DIS NOW\n");
        goto failed;
    }
    printf("*** Testing Down: Passed not taking bad sem ID! WE GOTTA END DIS NOW\n");
    printf("*** \n");


    /* 
        Test Close: bad id
    */
    // bad id
    fakeID = -1234;
    int closed = close(fakeID);
    if (closed != -1) {
        printf("Testing Close: failed not taking bad ID! WE GOTTA END DIS NOW\n");
        goto failed;
    }
    printf("*** Testing Close: Passed not taking bad ID! WE GOTTA END DIS NOW\n");
    printf("*** \n");

    goto passed;

    // failed sequence
failed:
    fd = open("/etc/OOP.txt", 0);
    one(fd);
    shutdown();
    return 0;

    // passed sequence
passed:
    fd = open("/etc/nah_id_win.txt", 0);
    one(fd);
    fd = open("/etc/OOP.txt", 0);
    one(fd);
    printf("*** HE IN FACT DID NOT WIN >:((((((((((((((((( BUT IT ISNT GOJOVERR TRUSTTT\n");
    shutdown();
    return 0;
}
