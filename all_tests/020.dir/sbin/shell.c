#include "libc.h"

int main(int argc, char** argv) {
    printf("*** you have made it into shell.c\n");

    printf("*** argc = %d\n",argc);
    for (int i=0; i<argc; i++) {
        printf("*** argv[%d]=%s\n",i,argv[i]);
    }

    int fd = open("/etc/end.txt", 0);

    char buf[100];

    read(fd, buf, 100);
    int id = fork();

    if(id == 0) {
       printf("*** so this is a child proccess in shell\n");  
       printf("*** Im boutta hit you with a double execl\n");
       int rc = execl("/sbin/bruh", 0);
       printf("*** execl failed, rc = %d\n",rc);
    } else {
        uint32_t status = 999;
        wait(id, &status);
        printf("*** yippeee!!\n");
    }
    

    return 2004;
}
