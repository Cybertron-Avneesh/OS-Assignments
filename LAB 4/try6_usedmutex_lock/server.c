#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>
#include <pthread.h>

struct msg_buffer
{
    int service_no;
    int PID;
    int service_input[4];
} message;

typedef struct{
    struct msg_buffer arr[10000];
    int front;
    int end;
    pthread_mutex_t lock;
}queue;

queue *q1;
int shmid;

void my_handler(){
    pthread_mutex_destroy(&q1->lock);       //destry mutex lock
    msgctl(shmid, IPC_RMID, NULL);          //Clearing request queue shared memory
    exit(1);
}

int main()
{
    signal(SIGINT, my_handler);

    int key1 = ftok("buffer.txt", 200);
    shmid = shmget(key1, sizeof(queue), IPC_CREAT | 0666);
    q1 = (queue *)shmat(shmid, NULL, 0);
    q1->front=0;
    q1->end=0;
    pthread_mutex_init(&q1->lock,NULL);
    //sleep(30);          //important command to check for working of queue, uncomment only if want to see queue working
    while (1)
    {
        while(q1->front>=q1->end);
        int front=q1->front;
        q1->front++;        //pop command in queue
        int pid = fork();
        if (pid < 0)
        {
            perror("Fork Error.");
            exit(1);
        }
        else if (pid == 0)
        {
            char argv_1[50];
            char argv_2[50];
            char argv_5[50];
            sprintf(argv_1, "%d", q1->arr[front].service_input[0]);
            sprintf(argv_2, "%d", q1->arr[front].service_input[1]);
            sprintf(argv_5, "%d", q1->arr[front].PID);
            int service_number=q1->arr[front].service_no;
            if(service_number==2)
                execl("./service2", "./service2", argv_1, argv_2, argv_5, NULL);
            else if(service_number==3)
                execl("./service3", "./service3", argv_1, argv_2, argv_5, NULL);
            else if(service_number==1){ 
                char argv_3[50];
                char argv_4[50];
                sprintf(argv_3, "%d", q1->arr[front].service_input[2]);
                sprintf(argv_4, "%d", q1->arr[front].service_input[3]);
                execl("./service1", "./service1", argv_1, argv_2, argv_3, argv_4, argv_5, NULL);
            }
            else{
                printf("Invalid Option\n");
                exit(1);
            }            
        }
    }
    return 0;
}