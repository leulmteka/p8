#include "libc.h"

int main(int argc, char** argv) {
    int pentagon = open("/etc/pentagon.txt",0);
    printf("*** this is a pentagon\n");
    cp(pentagon,2);
    printf("*** \n");

    int robot = open("/etc/robot.txt",0);
    printf("*** this is a robot\n");
    cp(robot,2);
    printf("*** \n");

    printf("*** seek at offset: %d\n",(int)seek(robot, 20));
    cp(robot,2);

    shutdown();
    return 0;
}
