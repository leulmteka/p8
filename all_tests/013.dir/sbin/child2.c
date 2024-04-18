#include "libc.h"

int main(int argc, char** argv) {
    printf("*** argc: %d\n",argc);
    
    int open_id = open("file3.txt", 0);
    char buffer[len(open_id) + 1];

    buffer[len(open_id)] = 0;
    read(open_id, buffer, len(open_id));
    printf("%s\n", buffer);

    return 72;

}