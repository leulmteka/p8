#include "libc.h"

void one(int fd) {
    cp(fd,2);
}

int main(int argc, char** argv) {
	// Lets use file reading for our ABCs
	int fd = open("/data/ABCs.txt",0);
	int monk = open("/data/LittleMonkeys.txt", 0);
    one(fd);
	close(fd);
	printf("*** closing\n");
	close(monk);
	int semaphores[4];
	for(int i=0;i<4;i++) {
		semaphores[i] = sem(i);
		if(semaphores[i] < 0){
			printf("Semaphores arent working");
			return 1;
		}
	}

	int finalsem = sem(1);
	printf("*** You can create Semaphores!\n");
	one(monk);
	printf("*** closing\n");
	close(monk);
	//sike we aint closing yet

	// for(int i=0;i<4;i++){
	// 	int chIlD = fork();
	// 	if(chIlD < 0){
	// 		printf("Forking isnt working");
	// 		return 1;
	// 	}
	// 	if(chIlD == 0){
	// 		for(int j=0;j<=i;j++) down(semaphores[i]);
	// 		printf("*** CHILD %d\n", i);
	// 		//printf("*** little monkey(s) jumping on the bed\n");
	// 		// 	one(monk);
	// 		if(i<3){
	// 			up(semaphores[i+1]);
	// 		}else{
	// 			up(finalsem);
	// 		} 
	// 		exit(0);
	// 	}	
	// }
	//printf("*** PARENT\n");
	// printf("*** little monkey(s) jumping on the bed\n");
	// one(monk);
	down(finalsem);
    shutdown();
    return 0;
}
