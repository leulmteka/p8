#include "libc.h"
// #include "debug.h"

void one(int fd) {
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));
}

int main(int argc, char** argv) {
    // you should make sure that you are not allowing char* buffers that are in the kernel space or in the kConfig.ioPic or kConfig.localPic
    char** buf1 = (char**) 0x2000; // the address of this char is now in kernel space
    *buf1 = "I am in the kernel space.";
    
    char** buf2 = (char**) 0xfec00000;// the address of this char is now the kConfig.ioAPIC
   // *buf2 = "I am the IOAPIC";

    char** buf3 = (char**) 0xfee00000; // the address of this char is now the kConfig.localAPIC
    //*buf3 = "I am the LAPIC";

    int fd_a = open("/etc/data.txt",0);
    one(fd_a);

    // because our buf array is in disallowed addresses, read(), write() and execl() should return -1 and prevent the system from using 
    // this buffer
    // testing read()
    int result = read(fd_a, buf1, 100); // is this the proper way to handle this error
    printf("*** read result = %d\n", result);
    printf("*** %s\n", *buf1); // if you are failing this check, you are most likely not preventing you a read for invalid address spaces

    result = read(fd_a, buf2, 100); 
    printf("*** read result = %d\n", result);
    char* string;
    if (result == -1) {
        string = "I am the IOAPIC";
    } else { // if it is not -1, we did not do it right
        string = *buf2;
    }
    printf("*** %s\n", string); 

    result = read(fd_a, buf3, 100);

    printf("*** read result = %d\n", result);   
    if (result == -1) {
        string = "I am the LAPIC";
    } else { // if it is not -1, we did not do it right
        string = *buf3;
    }
    printf("*** %s\n", string); 


    // testing write()
    result = write(fd_a, buf1, 100); 
    printf("*** write result = %d\n", result);

    result = write(fd_a, buf2, 100); 
    printf("*** write result = %d\n", result);

    result = write(fd_a, buf3, 100); 
    printf("*** write result = %d\n", result);

    // testing execl()
    result = execl("/sbin/shell", "shell", *buf1, *buf2, *buf3, 0);
    printf("*** execl result = %d\n", result);

    close(fd_a);

    shutdown();
    return 0;
}
