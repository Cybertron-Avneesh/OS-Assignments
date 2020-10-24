#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
int gcd(int a, int b){ 
    if (b == 0) 
        return a; 
    return gcd(b, a % b); 
}
void Sigusr1_handler(){}
int main(int argc,char * argv[]){
    signal(SIGUSR1,Sigusr1_handler);
    printf("Prog3 started...\n");
    int buffer[2];
    for(int i=1;i<3;i++){
        buffer[i-1]=atoi(argv[i]);
    }
    int g = buffer[0];
    g = gcd(g, buffer[1]);
    int lcm=buffer[0] * buffer[1];
    lcm/=g;
    int pid;
    if(argc==4)
        pid = atoi(argv[argc - 1]);
    int key = ftok("buffer.txt", pid);
    int shmid = shmget(key, 2 * sizeof(int), IPC_CREAT | 0666);
    int *result_buffer = (int *)shmat(shmid, NULL, 0);
    result_buffer[0] = lcm;
    result_buffer[1] = g;
    
    printf("Sending signal to pid = %d\n",pid);
    kill(pid, SIGUSR1);
    printf("Prog3 is exiting...\n\n");
    return 0;
}