#include "libc.h"

/*
    - tests all syscalls except write, fork + sem combo, and whether 
    there are checks for invalid desc. table entries
*/

void semtests() {
    int sem0 = sem(1);
    int sem1 = sem(1);
    for (int i = 0; i < 100; ++i) {
        down(sem0);
        down(sem1);
        up(sem1);
        up(sem0);
    }
    int err = down(-1);
    if (err >= 0) {
        printf("*** didn't handle invalid sem descriptor for down correctly\n");
        shutdown();
    }
    printf("*** passed sem tests\n");
}

int main(int argc, char** argv) {
    printf("*** last 439 test case, lesgo...\n");
    
    int fd = open("/etc/pain", 0);
    if (fd != 3) {
        printf("*** i think u forgot the std convention\n");
        shutdown();
    }

    // test fork
    int id = fork();
    if (id < 0) {
        printf("*** fork error in c1-1\n");
    } else if (id == 0) {
        int rc = execl("/sbin/c1", "c1", "1", 0);
        printf("*** execl failed for c1-1!, rc = %d\n", rc);
    } else {
        uint32_t rcode = -1;
        wait(id, &rcode);

        printf("*** c1 return code 1: %ld\n", rcode);
    }

    // test fork with shared semaphore and different args
    int s = sem(1);
    down(s);
    int id1 = fork();
    if (id1 < 0) {
        printf("*** fork error in c1-2\n");
    } else if (id1 == 0) {
        down(s);
        int rc = execl("/sbin/c1", "c1", "1", "2", "3", "4", "5", 0);
        printf("*** execl failed for c1-2!, rc = %d\n", rc);
    } else {
        uint32_t rcode1 = -1;
        up(s);
        wait(id1, &rcode1);

        printf("*** c1 return code 2: %ld\n", rcode1);
    }

    // sem test function above
    semtests();

    // check valid close and invalid close w invalid file descriptor
    int crc1 = close(fd);
    int crc2 = close(-1);
    if (crc1 < 0) {
        printf("*** pain didn't close correctly?\n");
        shutdown();
    }
    if (crc2 >= 0) {
        printf("*** didn't handle invalid file descriptor for close correctly\n");
        shutdown();
    }

    int fdp = open("/etc/pain", 0);
    // random seeks
    seek(fdp, 0);
    seek(fdp, 2);
    seek(fdp, 100000);
    seek(fdp, 0);

    // check len syscall
    int fdt = open("/etc/meow", 0);
    printf("*** len of meow: %d\n", len(fdt));

    // make sure you cant read to lapic, ioapic dmapped mem
    int apicr = read(fdt, (uint32_t*)0xfee00000, 1);
    int ioapicr = read(fdt, (uint32_t*)0xfec00000, 1);
    if (apicr >= 0 || ioapicr >= 0) {
        printf("*** reading into already mapped addresses\n");
        shutdown();
    }

    int cfdt = close(fdt);
    int cfdp = close(fdp);
    if (cfdt < 0 || cfdp < 0) {
        printf("*** error closing valid file\n");
        shutdown();
    }

    // make sure there are checks for invalid elf file and it's handled correctly
    execl("/etc/meow", "this is wrong", 0);
    printf("*** execl returned correctly\n");

    // check if seek and read work appropriately
    int fr = open("etc/pain", 0);
    int sz = len(fr) - 8009;
    seek(fr, 8009);
    char buff[sz + 1];
    buff[sz] = '\0';
    int finalread = read(fr, buff, sz);
    if (finalread < 0) {
        printf("couldn't do valid read call\n");
        shutdown();
    }
    printf("***\n");
    printf("%s\n", buff);
    printf("***\n");
    close(fr);

    printf("*** looks good, nice!\n");
    shutdown();
    return 0;
}
