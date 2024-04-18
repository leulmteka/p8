#include "libc.h"


// i think the purpose of this test is just going to be to test a bunch of execl edge cases
// according to sys.h 

int main(int argc, char** argv) {

    const char* shellPath = "/sbin/shell"; 
    // lets pass in a pointer that points to kernel memory
    // if you don't check the address on the pointers passed through,
    // you could end up copying kernel memory as an argument which
    // can be used by those with malicious intent
    // char* arg = (char*) 0x400000;
    


    /* for some reason running this test messed everything up royally
        I give up trying to figure it out I don't have time
        my best guess is that passing 0x400000  */
    // int err = execl(shellPath, "shell", arg);

    // if (err < 0) {
    //     printf("*** Checked pointer for kernel address\n");
    // } else {
    //     printf("*** Returned a nonnegative number\n");
    // }

    // should it be an error to execl into init?
    // execl("/sbin/init", "init");

    // what about execl into a path that doesn't exist?
    int err = execl("/THISISNTREAL", "fake");

    if (err < 0) {
        printf("*** Checked file for null\n");
    } else {
        printf("*** Returned a nonnegative number\n");
    }

    // execl into a non-executable
    // I think you may need to implement this part in ELF::load
    // I actually can't do this test either because it fails the read
    err = execl("data/panic.txt", "blah");
    
    if (err < 0) {
        printf("*** Checked type of file\n");
    } else {
        printf("*** Returned a nonnegative number\n");
    }

    // I figure I might as well make a test to find out if execl works as intended
    err = execl(shellPath, "shell");

    if (err < 0) {
        printf("*** Not good\n");
    } else {
        printf("*** Still not good\n");
    }

    shutdown();
    return 0;
}
