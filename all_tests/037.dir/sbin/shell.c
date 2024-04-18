#include "libc.h"

int stringToInt(const char *str)
{
    int result = 0;
    const char *ptr = str;
    while (*ptr != '\0')
    {
        if (*ptr >= '0' && *ptr <= '9')
        {
            result = result * 10 + (*ptr - '0');
        }
        else
        {
            printf("Invalid character found: ");
            return 0;
        }
        ++ptr;
    }
    return result;
}

int main(int argc, char** argv) {
    printf("argc = %d\n",argc);
    for (int i=0; i<argc; i++) {
        printf("argv[%d]=%s\n",i,argv[i]);
    }
    int fd = open(argv[1],0);
    if(seek(fd, stringToInt(argv[2])) < 0 ){
        return -666;
    }
    cp(fd,2,25);
    return 100;
}
