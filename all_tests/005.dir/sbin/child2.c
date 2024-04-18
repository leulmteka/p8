#include "libc.h"

int main(int argc, char** argv) {
    printf("*** argc: %d\n",argc);
    
    int fd = open("moose1.txt", 0);
    char buffer[len(fd) + 1];

    buffer[len(fd)] = 0;
    read(fd, buffer, len(fd));
    printf("%s\n", buffer);

    int id = fork();

    if (id < 0) {
        printf("*** fork failed!\n");
    } else if (id == 0) {
        printf("*** I get to print second!\n");
        int rc = execl("/sbin/child1", "child1", "c", "h", "i", "l", "d", 0);
        printf("*** execl failed, rc = %d\n",rc);
    } else {
        uint32_t status = -1;
        wait(id, &status);
        printf("*** back from wait %ld\n",status);
    }

    printf("*** child with child returning!\n");

    return 102;

}