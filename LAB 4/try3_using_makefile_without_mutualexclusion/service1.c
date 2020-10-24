#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>

int gcd(int a, int b){ 
    if (b == 0) 
        return a; 
    return gcd(b, a % b); 
}

void my_handler() {}

int main(int argc, char *argv[])
{
    signal(SIGUSR1, my_handler);
    printf("Prog1 started...\n");
    int buffer[4];
    for(int i=1;i<5;i++){
        buffer[i-1]=atoi(argv[i]);
    }

    int g = buffer[0];
    g = gcd(g, buffer[1]);
    int lcm_1=buffer[0] * buffer[1];
    lcm_1/=g;

    g = buffer[2];
    g = gcd(g, buffer[3]);
    int lcm_2=buffer[2] * buffer[3];;
    lcm_2/=g;

    g = lcm_1;
    g = gcd(g, lcm_2);
    int lcm=lcm_1*lcm_2;
    lcm/=g;
    int pid;
    if(argc==6)
        pid = atoi(argv[argc - 1]);
    int key = ftok("buffer.txt", pid);
    int shmid = shmget(key, 2 * sizeof(int), IPC_CREAT | 0666);
    int *result_buffer = (int *)shmat(shmid, NULL, 0);
    result_buffer[0] = lcm;
    
    printf("Sending signal to pid = %d\n",pid);
    kill(pid, SIGUSR1);
    printf("Prog1 is exiting...\n\n");
    return 0;
}