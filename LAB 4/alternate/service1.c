#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

#include <signal.h>
#include <sys/shm.h>
int gcd(int a, int b)
{

    if (a == 0)
        return b;
    if (b == 0)
        return a;

    if (a == b)
        return a;

    if (a > b)
        return gcd(a - b, b);
    return gcd(a, b - a);
}
void my_handler() {}
int main(int argc, char *argv[])
{
    signal(SIGUSR1, my_handler);
    int i;
    printf("My arguments are:-\n");
    int g = atoi(argv[1]);
    for (i = 2; i < argc - 1; i++)
    {
        g = gcd(g, atoi(argv[i]));
    }
    int pid = atoi(argv[argc - 1]);
    int key = ftok("buffer.txt", pid);
    int shmid = shmget(key, 2 * sizeof(int), IPC_CREAT | 0666);
    int *result_buffer = (int *)shmat(shmid, NULL, 0);
    result_buffer[0] = g;
    
    printf("%d\n",pid);
    kill(pid, SIGUSR1);
    printf("prog1 is exiting...\n");
    return 0;
}