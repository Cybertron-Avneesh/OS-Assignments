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

struct msg_buffer{
    int service_no;
    int PID;
    int shmid;
    int service_input[4];
} message;

typedef struct{
    struct msg_buffer arr[100000];
    int front;
    int end;
    pthread_mutex_t lock;
}queue;

queue *q;
int shmid_queue;
void create_shared_memory(){
    int key_queue = ftok("buffer.txt", 10);
    if(key_queue<0){
	    perror("error0:");
	 	exit(1);
	}
    shmid_queue = shmget(key_queue, sizeof(queue), IPC_CREAT | 0666);
    if(shmid_queue<0){
		perror("error1:");
		exit(1);
	}
    q = (queue *)shmat(shmid_queue, NULL, 0);
    if(q == (void *) -1){
		perror("error2:");
		exit(1);
	}
}
void my_handler(){
    pthread_mutex_destroy(&q->lock);       //destry mutex lock
    shmctl(shmid_queue, IPC_RMID, NULL);   //Clearing request queue shared memory
    printf("Server ended\n");
    exit(1);
}

int main(){
    printf("Server started\n");
    signal(SIGINT, my_handler);
    create_shared_memory();
    q->front=0;
    q->end=0;
    pthread_mutex_init(&q->lock,NULL);
    char argv_1[50];
    char argv_2[50];
    char argv_3[50];
    char argv_4[50];
    char argv_5[50];
    char argv_6[50];
    //sleep(30);          //important command to check for working of queue, uncomment only if want to see queue working
    while (1){
        if(q->end==99999 || q->front==99999) {
            while(pthread_mutex_trylock(&q->lock)!=0);
            if(q->end==99999)       //When queue array completes then we need to initialize value of front and end of queue
                q->end=0;
            if(q->front==99999)
                q->front=0;
            pthread_mutex_unlock(&q->lock);
        }
        while(q->front==q->end);
        while(pthread_mutex_trylock(&q->lock)!=0);  //lock
            int front=q->front;
            q->front++;        //pop command in queue
            sprintf(argv_1, "%d", q->arr[front].service_input[0]);
            sprintf(argv_2, "%d", q->arr[front].service_input[1]);
            sprintf(argv_5, "%d", q->arr[front].PID);
            sprintf(argv_6,"%d",q->arr[front].shmid);
            int service_number=q->arr[front].service_no;
            if(service_number==1){ 
                sprintf(argv_3, "%d", q->arr[front].service_input[2]);
                sprintf(argv_4, "%d", q->arr[front].service_input[3]);
            }
        pthread_mutex_unlock(&q->lock);     //unlock
        int pid = fork();
        if (pid < 0){
            perror("Fork Error.");
            exit(1);
        }
        else if (pid == 0){
            if(service_number==2)
                execl("./service2", "./service2", argv_1, argv_2, argv_5, argv_6, NULL);
            else if(service_number==3)
                execl("./service3", "./service3", argv_1, argv_2, argv_5, argv_6, NULL);
            else if(service_number==1){
                execl("./service1", "./service1", argv_1, argv_2, argv_3, argv_4, argv_5, argv_6, NULL);
            }
            else{
                printf("Invalid Option\n");
                exit(1);
            }            
        }
    }
    return 0;
}