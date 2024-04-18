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

void cp(int from, int to, int numBytes)
{
    while (numBytes > 0)
    {
        char buf[1024]; // Buffer size can be adjusted as needed
        ssize_t n = read(from, buf, sizeof(buf) < numBytes ? sizeof(buf) : numBytes);
        if (n <= 0)
        {
            if (n < 0)
            {
                printf("*** %s:%d read error, fd = %d\n", __FILE__, __LINE__, from);
            }
            break;
        }
        char *ptr = buf;
        ssize_t remaining = n;
        while (remaining > 0)
        {
            ssize_t m = write(to, ptr, remaining);
            if (m < 0)
            {
                printf("*** %s:%d write error, fd = %d\n", __FILE__, __LINE__, to);
                break;
            }
            remaining -= m;
            ptr += m;
        }
        numBytes -= n;
    }
}
