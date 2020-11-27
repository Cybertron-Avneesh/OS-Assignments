#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>

int gcd(int a, int b);
void my_handler() {}

int main(int argc, char *argv[]){
    signal(SIGUSR1, my_handler);
    printf("Service 1 started...\n");
    int buffer[4];
    for(int i=1;i<5;i++){
        buffer[i-1]=atoi(argv[i]);
    }
    int lcm=buffer[0];
    for(int i=1;i<4;i++){
        lcm=((buffer[i]*lcm)/gcd(buffer[i],lcm));
    }
    int pid = atoi(argv[argc - 2]);
    int shmid = atoi(argv[argc - 1]);
    int *result_buffer = (int *)shmat(shmid, NULL, 0);
    if(result_buffer == (void *) -1){
		perror("error2:");
		exit(1);
	}
    result_buffer[0] = lcm;
    //sleep(5);          //important command to check for working of queue, uncomment only to see queue working
    printf("Sending signal to pid = %d\n",pid);
    kill(pid, SIGUSR1);
    printf("Service 1 is exiting...\n\n");
    return 0;
}

int gcd(int a, int b){ 
    if (b == 0) 
        return a; 
    return gcd(b, a % b); 
}