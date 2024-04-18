#include "libc.h"

int main(int argc, char **argv)
{
    int fd = open("favorites.dir/teenage dream.txt", 0);
    char buf[31];
    read(fd, buf, 31);
    printf("*** %s\n", buf);

    // make sure the files copied over from the previous
    printf("*** previous song << \n");
    char buf2[43];
    read(0, buf2, 43);
    printf("*** %s\n\n", buf2);

    return 19;
}
