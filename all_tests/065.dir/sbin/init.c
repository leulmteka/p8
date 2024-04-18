#include "libc.h"

#define NULL 0

int main(int argc, char** argv) {

    printf("*** Testing your kernel's memory protection!\n");
    printf("***\n");
    printf("*** When a user program does something bad, like try to access an\n");
    printf("*** invalid pointer, it shouldn't cause the kernel to panic -- it\n");
    printf("*** should instead force the program to exit.\n");
    printf("***\n");

    printf("*** #### Begin tests:\n");

    uint32_t id;
    uint32_t status;



    printf("*** ## Test 0: checking that fork works.\n");

    if (!(id = fork())) {
        printf("*** in child\n");
        exit(0);
    }

    wait(id, &status);
    // This test doesn't check status or ids, but this output may be useful to you
    printf("Child %ld exited with status %ld\n", id, status);
    printf("*** ## end test 0\n***\n");



    printf("*** ## test 1: Attempting to read kernel memory.\n");

    if (!(id = fork())) {
        printf("*** in child\n");

        uint32_t* kernel_start = (uint32_t*) 0x8000;
        uint32_t* kernel_end = (uint32_t*) (6 * 1024 * 1024);
        volatile uint32_t *ptr = (volatile uint32_t*) kernel_start;

        uint32_t checksum = 0;
        while (ptr < kernel_end) checksum = checksum ^ *(ptr++);

        printf("*** Values: %ld\n", checksum);
        printf("*** Managed to read kernel memory\n");
        exit(1);
    }

    wait(id, &status);
    printf("Child %ld exited with status %ld\n", id, status);
    printf("*** ## end test 1\n***\n");



    printf("*** ## test 2: Attempting to write to kernel memory.\n");
    printf("*** (this test is likely to hang or crash if your kernel fails it)\n");

    if (!(id = fork())) {
        printf("*** in child\n");

        uint32_t* kernel_start = (uint32_t*) 0x8000;
        uint32_t* kernel_end = (uint32_t*) (6 * 1024 * 1024);
        volatile uint32_t *ptr = (volatile uint32_t*) kernel_start;

        printf("*** Filling kernel memory with zeroes...\n");

        // Fill kernel memory with zeroes
        while (ptr < kernel_end) {
            *(ptr++) = 0;
        }

        // Note that this print statement will almost certainly crash
        printf("*** Managed to overwrite kernel memory\n");
        exit(2);
    }

    wait(id, &status);
    printf("Child %ld exited with status %ld\n", id, status);
    printf("*** ## end test 2\n***\n");




    printf("*** ## test 3: Attempting null-pointer dereference.\n");

    if (!(id = fork())) {
        printf("*** in child\n");
        volatile uint32_t* a = (volatile uint32_t*) 0;

        uint32_t value = *a;
        printf("*** Value: %ld\n", value);
        printf("*** Managed to read from the null page???\n");

        *a = 15;
        printf("*** Managed to write to the null page???\n");
        exit(3);
    }

    wait(id, &status);
    printf("Child %ld exited with status %ld\n", id, status);
    printf("*** ## end test 3\n");



    printf("*** ## test 4: Execl argument checks.\n***\n");

    if (!(id = fork())) {
        printf("*** in child\n");

        int res;

        // path is null, so this should fail, but not crash your kernel
        printf("*** calling execl(NULL, NULL)\n");
        res = execl(NULL, NULL);
        printf("*** success\n");

        // file doesn't exist, so this should fail
        printf("*** calling execl(\"/sbin/not-init\", \"not-init\", NULL)\n");
        res = execl("/sbin/not-init", "not-init", NULL);
        printf("*** success\n");

        exit(4);
    }

    wait(id, &status);
    printf("Child %ld exited with status %ld\n", id, status);
    printf("*** ## end test 4\n");


    // There are a ton of other things that I didn't check here --
    // What if the user passes a string that points to kernel memory?
    // What if the user calls read() with the buffer in kernel memory?
    // What if the user calls write() with the buffer in kernel memory?
    // The same applies to any syscall that takes pointers as an argument,
    // even ones that don't seem like they do.  (Remember the x86 calling
    // convention!)

    // Make sure that you take security and isolation into account when
    // writing code; it's incredibly important in a kernel, but it's also
    // important in any software.  Don't trust user input!


    shutdown();
    return 0;
}
