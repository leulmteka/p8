#include "libc.h"

int main(int argc, char** argv) {
    
    // does your sem creation work?
    int sem1 = sem(2);

    // does your fork work
    int id = fork();

    if (id < 0) {
        printf("fork failed");
    } else if (id == 0) {
        // if your fork works can you do a fork within a forked process? 
        int id2 = fork();

        if(id2 == 0) {
            /* granchild */
            // Let's see of your get semaphore and down/up is working
            // If you get a jumble of rearranged output here, it is likely you have to change gheith code
            down(sem1);
            down(sem1);
            printf("*** I am the grandchild, I was born in 2004 and I have the lock\n");
            up(sem1);
            // does your execl work? And do you properly push arguments onto the stack
            int rc = execl("/sbin/shell","she","ab",0);
            printf("*** execl failed, rc = %d\n",rc);
            
        }
        /* child */
        uint32_t status = 42;
        wait(id2, &status);
        printf("*** we are finally back in init.c\n");
        printf("*** I am the child and I waited for my own child <3: %ld\n", status);
    } else{
        // this is a selfish parent and decides to print before waiting
        printf("*** HAHA I am the parent and I did not wait, but now i have to i guess!\n");
        uint32_t status = 42;
        wait(id, &status);
        int closeval = close(id);
        if(closeval == 0) {printf("*** I have disowned my child now for NO reason!");}
    }
    

    // one last test, if you page fault here you are accessing memory you should NOT be accessing(kernel memory)
    // anything less than 0x00600000 should be off limits!!! Along with ioApic and localApic
    int fd = open("/etc/end.txt",0);
    
    char * devious_buf = (char*) 0x00300000;
    read(fd, devious_buf, 50);
    
    // make sure you check if you're reading an invalid address in your open as well!
    fd = open((const char*) 0x00300000, 0);
    if(fd == -1){
        printf("*** you have appropriate security measures!!!\n");
    }

    shutdown();
    return 0;
}
