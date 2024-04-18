#include "libc.h"

int main(int argc, char** argv) {

    // Taken from t0, just simply printing out the arguments besides the program name to do a clever
    // intro to this section of the tests.

    for (int i=1; i<argc; i++) {
        printf("*** %s\n",argv[i]);
    }
    printf("*** ====== Execl Test Cases: ======\n");
    printf("*** Passed the first test (since you're here!)\n");

    /** 
     * Now this is quit possibly the most difficult part of this test, we CANNOT execute shell.c (this file) 
     * because it's not an ELF! Therefore, we have to implement some kind of check in our ELF.cc, ELF.hh, Sys.cc
     * or a combination of the three. I recommend double checking the format of an ELF file, Gheith briefly covered
     * it in class but it is documented in the code for us.
     * 
     * Even after implementing your check, you may notice you're hanging... Did you do something too soon
     * that you have to wait to check the validity of the file before doing?
     * 
     * Here's a hint, even if the file we were looking for would be an ELF, if the file was NULL or a directory
     * you'd have the same issue. Maybe think about why we're hanging, why can't we return from the execl sys call...
     * Has something been modified?
    */

    int execl_check = execl("/sbin/shell.c", "uh oh", "that's not an ELF", 0);
    if (execl_check >= 0) {
        printf("*** How are you even here... that's not good...\n");
        shutdown();
    }
    printf("*** Successfully Handled Non-ELF File!\n");

    /**
     * And again, we have some pointer manipulation. Given all the other checks, I hope this is a simple fix
     * by now! We can't execute the APICs or the kernel individually, this is dangerous!
    */

    execl_check = execl("/sbin/shell.c", (const char*) 0xFEC00000, "that's the APIC!", 0);
    if (execl_check >= 0) {
        printf("*** How are you even here... that's not good...\n");
        shutdown();
    }
    printf("*** Successfully Handled Non-user Argument!\n");

    execl_check = execl((const char *) 0xFEC00000, "/sbin/shell.c", "that's the APIC again!", 0);
    if (execl_check >= 0) {
        printf("*** How are you even here... that's not good...\n");
        shutdown();
    }
    printf("*** Successfully Handled Non-user Path!\n");

    return 90210;
}
