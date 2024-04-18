#include "libc.h"

/*
    this test case tests a student's file related system calls aka open,
    close, read, write, seek, and len
*/

void one(int fd) {
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));

    cp(fd,2);
}

int main(int argc, char** argv) {
    printf("*** welcome to my pet shop! lets take a look at our lovely animals.\n");
    printf("\n");

    int cat = open("/cat/look.txt", 0);
    printf("*** this is our first animal! can you guess what it is?\n");
    printf("*** \n");
    cp(cat, 2);
    printf("\n");
    printf("*** \n");
    printf("*** it's a cat! cats say...\n");

    int cat_say = open("/cat/say.txt", 0);
    cp(cat_say, 2);
    printf("\n");

    printf("*** length of a cat: %d\n", len(cat));

    printf("*** bye bye kitty!\n");
    close(cat);
    printf("\n");
    printf("*** \n");

    int dog = open("/dog/look.txt", 0);
    printf("*** here is our second animal! what do you think it is?\n");
    printf("*** \n");
    cp(dog, 2);
    printf("\n");
    printf("*** \n");
    printf("*** it's a dog! dogs say...\n");

    int dog_say = open("/dog/say.txt", 0);
    cp(dog_say, 2);
    printf("\n");

    printf("*** length of a dog: %d\n", len(dog));

    printf("*** bye bye doggy!\n");
    close(dog);
    printf("\n");
    printf("*** \n");

    int bunny = open("/bunny/look.txt", 0);
    printf("*** here is our last animal!\n");
    printf("*** \n");
    cp(bunny, 2);
    printf("\n");
    printf("*** \n");

    int bunny_say = open("/bunny/say.txt", 0);
    cp(bunny_say, 2);
    printf("\n");
    
    printf("*** \n");
    seek(bunny, 40);
    cp(bunny, 2);
    printf("\n");
    printf("*** \n");

    printf("*** it's a carrot!!! how cute <3\n");

    printf("*** length of a bunny and a carrot: %d\n", len(bunny));

    printf("*** bye bye bunny!\n");
    close(bunny);
    printf("\n");
    printf("*** \n");

    shutdown();
    return 0;
}
