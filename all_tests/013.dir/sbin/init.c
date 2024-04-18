#include "libc.h"

int open_check(char* file_name) {
    return open(file_name, 0);
}

int main(int argc, char** argv) {
    /*
    We begin with a basic functionality test. Make sure that everything is implemented properly as per the documentation.
    */
    printf("*** argc: %d\n", argc);
    printf("*** argv: %s\n", argv[0]);

    printf("*** let's begin our test\n");
    printf("\n*** testing open\n");

    int num_tests_passed = 0;

    int open_id = open_check("file1.txt");
    if (open_id == 3) {
        num_tests_passed++;
        printf("*** open test complete \n");
    }

    printf("\n*** testing reads\n");
    char buf_read[len(open_id) + 1];
    read(open_id, buf_read, len(open_id));
    printf("%s\n", buf_read);
    printf("*** read test complete \n");
    num_tests_passed++;

    /*
    Make sure that you make sure the buffer is where it is supposed to be!
    */
    printf("\n*** let's check some harder reads\n");
    int read_result = read(open_id, (void*) 0, 100);
    if (read_result == -1) {
        printf("*** harder read test complete \n");
        num_tests_passed++;
    }
    read_result = read(open_id, (void*) 0x100, 100);
    if (read_result == -1) {
        printf("*** harder read test complete \n");
        num_tests_passed++;
    }
    read_result = read(open_id, (void*) 0x00500000, 100);
    if (read_result == -1) {
        printf("*** harder read test complete \n");
        num_tests_passed++;
    }
    
    char buf_seek[len(open_id) + 1];
    printf("\n*** testing seeks\n");
    seek(open_id, 10);
    seek(open_id, 5);
    seek(open_id, 4);
    read(open_id, buf_seek, len(open_id));
    printf("*** %s\n", buf_seek);
    printf("*** seek test complete \n");
    num_tests_passed++;

    /*
    Forking test
    */
    printf("\n*** time to start forking\n");
    int child_id = fork();
    if(child_id == 0) { // you are the child
        printf("*** child running \n");
        printf("*** child2 running \n");
        execl("/sbin/child1", "child1", "1", 0);
        printf("*** you messed up \n");
    } else if(child_id > 0) {
        uint32_t status = -1;
        wait(child_id, &status);
        printf("*** parent resuming \n");
    } else {
        printf("*** you messed up \n");
    }

    printf("*** fork test complete \n");
    num_tests_passed++;

    /*
    There are a couple of reserved entries. They can be closed. Make sure your code accounts for them.
    */
    printf("\n*** random test\n");
    printf("*** closing reserved entries...\n");
    close(0);
    close(2);
    int id1 = open_check("file1.txt");
    int id2 = open_check("file2.txt");
    int id3 = open_check("file3.txt");
    char rand_buf[len(id1) + 1];
    if(read(id1, rand_buf, len(id1)) != -1) {
        printf("*** random 1 complete\n");
        printf("%s\n", rand_buf);
        num_tests_passed++;
    }
    if(read(id2, rand_buf, len(id2)) != -1) {
        printf("*** random 2 complete\n");
        printf("%s\n", rand_buf);
        num_tests_passed++;
    }
    if(read(id3, rand_buf, len(id3)) != -1) {
        printf("*** random 3 complete\n");
        printf("%s\n", rand_buf);
        num_tests_passed++;
    }
    printf("*** num tests passed: %d \n", num_tests_passed);

    shutdown();
    return 0;
}
