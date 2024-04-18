#include "libc.h"

int putchar(int c) {
    char t = (char)c;
    return write(1,&t,1);
}

int puts(const char* p) {
    char c;
    int count = 0;
    while ((c = *p++) != 0) {
        int n = putchar(c); 
        if (n < 0) return n;
        count ++;
    }
    putchar('\n');
    
    return count+1;
}

//our call to cp from one() in init.c is cp(fd, 2)

/*
according to chatgpt:
The cp(int from, int to) function in libc.c is designed to copy data from one file descriptor to another in a Unix-like operating system. 
It reads chunks of data from the source file descriptor (from) and writes them to the destination file descriptor (to). 
This function uses a buffer of 100 bytes to temporarily store the data being transferred. 
*/
void cp(int from, int to) {

    //continue until finished copying data or error
    while (1) {
        char buf[100];

        //attempt to read up to 100 bytes from 'from' descriptor into 'buf' (returns number of bytes read as n)
        ssize_t n = read(from,buf,100);

        //end of file has been reached if n == 0
        if (n == 0) break;

        //ERROR during read operation
        if (n < 0) {
            printf("*** %s:%d read error, fd = %d\n",__FILE__,__LINE__,from);
            break;
        }

        //n is positive

        //ptr initially points to the start of buf
        char *ptr = buf;
        while (n > 0) {

            //attempts to write n bytes (up to 100 but can be less depending on what is left) from ptr (buf) to to descriptor
            //returns number of bytes actually written, stored as m
            //in this write process, it is printed to the terminal, so we get:
                //*** this is nice
                //*** we can read and write
            ssize_t m = write(to,ptr,n);

            //ERROR during write operation
            if (m < 0) {
                printf("*** %s:%d write error, fd = %d\n",__FILE__,__LINE__,to);
                break;
            }

            //keep track of how many bytes still need to be written
            n -= m;

            //ptr moves forward in buffer the number of bytes that have been written
            ptr += m;
        }
    }
}
