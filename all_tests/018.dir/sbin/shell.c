#include "libc.h"

int main(int argc, char** argv) {
    
    printf("*** argc = %d\n",argc);
    for (int i=0; i<argc; i++) {
        printf("*** argv[%d]=%s\n",i,argv[i]);
    }
    
    //how a lot of arguments
    int doom = open(argv[0],0x0);

    cp(doom, 1);
    
    shutdown();
    return 1;
}
