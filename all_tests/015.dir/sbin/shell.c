#include "libc.h"

int main(int argc, char** argv) {
    //printf("argc address %x\n", &argc);
    printf("*** argc = %d\n", argc);
    //printf("argc address %x\n", &argc);
    for (int i=0; i < argc; i++) {
        printf("*** argv[%d]=%s\n",i,argv[i]);
        //printf("argv address %lx\n", (uint32_t) argv[i]);
    }

    cp(3,1);

    return 666;
}
