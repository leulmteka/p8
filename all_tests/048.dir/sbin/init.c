#include "libc.h"



// refrenced test case t0 while creating this test case

int main(int argc, char** argv) {

    // testing open
    int fd = open("/eclipse/trump.txt",0);
    //tests read and write
    cp(fd, 2);
    // close the file
    printf("\n*** close = %d\n",close(fd));
    char buf[10];
    size_t s = read(fd, buf, 10);
    // trying to read a file thats already closed
    printf("\n***number returned = %d", s);

    // open a different file
    fd = open("/eclipse/glasses.txt",0);
    //tests read and write
    cp(fd, 2);
    // close the file
    printf("\n*** close = %d\n",close(fd));

    // open a different file
    fd = open("/data/data.txt",0);
    int x = (int) seek(fd, 5);
    printf("\n*** offset returned =  %d\n",x); 
    shutdown();
    return 0;
}
