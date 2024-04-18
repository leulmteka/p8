#include "libc.h"

int main(int argc, char** argv) {
    printf("*** you only want to be here once\n");
    
    shutdown();
    
    return 1;
}
