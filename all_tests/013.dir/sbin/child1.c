#include "libc.h"

int main(int argc, char** argv) {
    printf("*** argc: %d\n",argc);
    
    int open_id = open("file2.txt", 0);
    char buffer[len(open_id) + 1];
    seek(open_id, 0);

    buffer[len(open_id)] = 0;
    read(open_id, buffer, len(open_id));
    printf("%s\n", buffer);

    printf("*** another fork?? \n");
    int child2_id = fork();
    if(child2_id == 0) {
        printf("*** child2 running \n");
        execl("/sbin/child2", "child2", "1", 0);
    } else {
        uint32_t status = -1;
        wait(child2_id, &status);
        printf("*** child1 resuming \n");
    }

    return 420;

}