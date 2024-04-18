#include "libc.h"

/**
 * GOAL: test to see if you are properly vetting the ELF files being executed with execl()
 * 
 * NOTE: Would avoid recompiling the program with 'make' however if you need to, just call
 * "make test_setup" in order to setup the ELF files correctly (more like incorrectly)
 * 
 * IMPORTANT: DON'T DELETE THE .hex FILES! 
*/


void one(int fd) {
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));

    cp(fd,2);
}

int main(int argc, char** argv) {
    printf("*** %d\n",argc);
    for (int i=0; i<argc; i++) {
        printf("*** %s\n",argv[i]);
    }
    

    //execl that need to fail

    printf("*** Attempting to open file w/h invalid magic #'s...\n");
    execl("/sbin/sus0",0);
    printf("*** Checked for invalid magic #'s!\n");

    printf("*** Attempting to open file that's big-endian...\n");
    execl("/sbin/sus1",0);
    printf("*** Checked for endianess!\n");
    
    printf("*** Attempting to open file that's 64 bit addressed...\n");
    execl("/sbin/sus2",0);
    printf("*** Checked for bit mode!\n");

    printf("*** Attempting to open file targeted towards Linux Kernel...\n");
    execl("/sbin/sus3",0);
    printf("*** Checked for OS type!\n");

    printf("*** Attempting to open file with invalid version of ELF\n");
    execl("/sbin/sus4",0);
    printf("*** Checked for ELF Version!\n");

    printf("*** Attempting to execute an non-executible ELF file\n");
    execl("/sbin/sus5",0);
    printf("*** Checked for ELF file type\n");

    printf("*** Attempted to open an ELF with no program header\n");
    execl("/sbin/sus6",0);
    printf("*** Checked for Program Header offset\n");

    //run a proper ELF file
    printf("*** Running a proper ELF file\n");
    execl("/sbin/shell","/data/long_dir/Music/MFDoom/MM...FOOD/DOOM.txt",0);
    printf("*** Shouldn't be here...\n");
    shutdown();
    return 0;
}
