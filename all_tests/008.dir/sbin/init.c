#include "libc.h"

// tests fork and wait
int recurseFork(int f, int c, int s) {
    if (f > 0) {
        uint32_t stat = 40;
        wait(f, &stat);
        down(s);
        return stat;
    } else if (f == 0) {
        up(s);
        return (c > 5) ? 1 : recurseFork(fork(), c+1, s);
    } else {
        printf("*** spoon");
        return 0;
    }
}

int main(int argc, char** argv) {

    int d = open("/dirt",0);
    printf("*** open and len... %s\n", (len(d) == 45) ? "Check" : "Try Again");
    char* buf = (char*)0x80015000;
    int i = read(d, buf, 0);
    printf("*** read nothing... %s\n", (i == 0) ? "Check" : "Try Again");
    printf("%s", buf);

    int w = d; 
    
    // w = open("/water",0);
    // printf("%d\n",w);
    // printf("*** open a symlink... %s\n", (len(w) == 45) ? "Check" : "Try Again");

    d = open((const char*) 0x00300000, 0);
    printf("*** don't open the kernel... %s\n", (d == -1) ? "Check" : "Try Again");
    
    i = read(w, buf, 4);
    printf("*** read 4 chars... %s\n", (i == 4) ? "Check" : "Try Again");
    printf("%s", buf);
    printf("Did this line output with asterisks??\n");

    buf += 4;
    i = read(w, buf, 13);
    printf("*** read some more... %s\n", (i == 13) ? "Check" : "Try Again");
    printf("*** %s\n", buf);

    buf += 13;
    seek(w,0);
    i = read(w, buf, 50);
    printf("*** seek start... %s\n", (i == 45) ? "Check" : "Try Again");
    printf("%s", buf);

    buf += 45;
    seek(w,19);
    i = read(w, buf, 50);
    printf("*** seek 19... %s\n", (i == 26) ? "Check" : "Try Again");
    printf("*** %s", buf);
    
    char* mr_kern = (char*) 0x00501000;
    i = read(w, mr_kern, 5);
    printf("*** kernel memory intact... %s\n",i < 0 ? "Check" : "Try Again");

    int f = fork();
    int s = sem(2);
    int t = recurseFork(f,1,s);
    printf("*** fork, wait, and sem... %s\n", t > 0 ? "Check" : "Try Again");

    shutdown();
    return 0;
}

