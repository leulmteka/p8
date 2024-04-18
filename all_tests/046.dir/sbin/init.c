#include "libc.h"

int main()
{
    //test open and close
    int cannotOpen= open("/somefolder/somefilefile.txt", 0);
    int cannotClose = close(cannotOpen);
    if(cannotOpen < 0){
        printf("*** cool probably\n");
    }
    if(cannotClose < 0){
        printf("*** groggy\n");
    }
    

    //test reading from file which can't be opened
    char readTo[50];

    int cannotOpenTwo= open("/somefolder/somefilefile.txt", 0);

    ssize_t bytesRead = read(cannotOpenTwo, readTo, sizeof(readTo));
    
    if (bytesRead < 0)
    {
        printf("*** nice\n");
    }

    printf("*** done\n");
    shutdown();
    return 0;
}