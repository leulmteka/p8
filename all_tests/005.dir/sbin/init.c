#include "libc.h"

int char_to_int(char c) {
    return (int) (c - '0');
}

void sum(int fd, int freq) {
    char buffer[10];
    int counter = 0;

    // Ensure that your read() returns 0 when it reaches the end of the file
    // (meaning 0 bytes were read in)
    while(read(fd, buffer, 10) != 0) {
        int sum = 0;
        if (counter % freq == 0) printf("*** ");
        for (int i = 0; i < 10; ++i) {
            if (counter % freq == 0) printf("%d ", char_to_int(buffer[i]));
            if (i != 9 && counter % freq == 0) printf("+ ");
            sum += char_to_int(buffer[i]);
        }
        if (counter % freq == 0) printf(" = %d\n", sum);
        ++counter;
    }
}

int main(int argc, char** argv) {
    printf("*** argc: %d\n", argc);
    printf("*** argv: %s\n", argv[0]);
    printf("***\n");

    printf("*** let's add some numbers\n");

    int sum_fd = open("sum.txt", 0);
    if (sum_fd != 3) shutdown();

    sum(sum_fd, 15);
    printf("***\n");

    printf("*** let's see some art!\n");

    int moose_fd = open("moose1.txt", 0);
    if (moose_fd != 4) shutdown();

    seek(moose_fd, 100);
    seek(moose_fd, 1000);
    seek(moose_fd, 0);

    char moose_buffer[len(moose_fd) + 1];
    moose_buffer[len(moose_fd)] = 0;
    read(moose_fd, moose_buffer, len(moose_fd));
    printf("%s\n", moose_buffer);
    printf("***\n");

    printf("*** let's try to corrupt your kernel!\n");

    // attempt to read the file into the kernel's address space
    // this should fail and return -1

    // none of these should page fault because any attempt to access
    // parts of the kernel's space should be FAIL immediately. 

    int init_fd = open("sbin/init", 0);
    if (init_fd != 5) shutdown();

    for (uint32_t addr = 0x00000000; addr < 0x00600000; addr += 0x1000) {
        seek(moose_fd, 0);
        void *illegal = (uint32_t *) addr;

        // try to load some art into the kernel (maybe the kernel wants some)
        int result = read(moose_fd, illegal, len(moose_fd));
        if (result != -1) shutdown();

        // try to load this program into the kernel's space
        result = read(init_fd, illegal, len(init_fd));
        if (result != -1) shutdown();
    }

    printf("*** your kernel survived... for now\n");
    printf("***\n");

    printf("*** fork test!\n");
    
    int id = fork();

    if (id < 0) {
        printf("*** fork failed!\n");
    } else if (id == 0) {
        printf("*** in child process\n");
        int rc = execl("/sbin/child1", "child1", "c", "h", "i", "l", "d", 0);
        printf("*** execl failed, rc = %d\n",rc);
    } else {
        uint32_t status = -1;
        wait(id, &status);
        printf("*** back from wait %ld\n",status);
    }

    close(id);
    printf("***\n");

    printf("*** fork & semaphore test!\n");

    // this is a simple test to ensure that your semaphore works
    // the child should NOT print before the parent

    int sd = sem(1);
    down(sd);

    id = fork();

    if (id < 0) {
        printf("*** fork failed!\n");
    } else if (id == 0) {
        down(sd);
        printf("*** I get to print second!\n");
        int rc = execl("/sbin/child2", "child2", "c", "h", "i", "l", "d", 0);
        printf("*** execl failed, rc = %d\n",rc);
    } else {
        printf("*** I get to print first!\n");
        up(sd);
        uint32_t status = -1;
        wait(id, &status);
        printf("*** back from wait %ld\n",status);
    }

    printf("*** original parent shutting down!\n");


    shutdown();
    return 0;
}
