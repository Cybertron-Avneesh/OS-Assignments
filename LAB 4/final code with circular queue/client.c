#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>
#include <pthread.h>

// Define message queue structure
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
void create_shared_memory(){
    key_t key_queue = ftok("buffer.txt", 10);
    if(key_queue<0){
	    perror("error0:");
	    exit(1);
	}
    int shmid_queue = shmget(key_queue, sizeof(queue), IPC_CREAT | 0666);
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
int buffer[4];
void scan_buffer(int n);
void print_result(int type, int lcm, int gcd);

void my_handler(){}
int main(){
    printf("Client Process started\n");
    printf("PID of this client process is: %d\n",getpid());
    signal(SIGUSR1, my_handler);
    create_shared_memory();
    int service_type=0;
    while(service_type<1 || service_type>3){
      printf("Enter service type: \n");
      printf("1-> LCM of 4 integers\n");
      printf("2-> GCD of 2 integers\n");
      printf("3-> LCM and GCD of 2 integers\n");
      scanf("%d",&service_type);
    }
    message.service_no=service_type;
    switch(service_type){
        case 1:     
            scan_buffer(4);
            break;
        case 2:     
        case 3:     
            scan_buffer(2);
            break;
        default:    
            printf("Invalid choice\n");
            exit(-1);
    }

    message.PID = getpid();
    key_t key = ftok("buffer.txt", getpid());       //Key generated can be negative, but if it is invlid then later on shmid will be negative
    int shmid = shmget(key, 2 * sizeof(int), IPC_CREAT | 0666);
    if(shmid<0){
		perror("error1:");
		exit(1);
	}
    int *result_buffer = (int *)shmat(shmid, NULL, 0);
    if(result_buffer == (void *) -1){
		perror("error2:");
		exit(1);
	}

    while(pthread_mutex_trylock(&q->lock)!=0);     //lock mutex
        q->arr[q->end].PID = getpid();        //pushing items into queue
        q->arr[q->end].service_no = service_type;
        q->arr[q->end].shmid=shmid;
        for(int i=0;i<4;i++)
            q->arr[q->end].service_input[i]=buffer[i];
        q->end++;
    pthread_mutex_unlock(&q->lock);        //unlock mutex

    pause();
    switch(service_type){
        case 1:     
            print_result(1,result_buffer[0],0);
            break;
        case 2:     
            print_result(2,0,result_buffer[0]);
            break;
        case 3:     
            print_result(3,result_buffer[0],result_buffer[1]);
            break;
        default:    
            printf("Invalid choice\n");
            exit(-1);
    }

    shmctl(shmid, IPC_RMID, NULL);  //free result_buffer from memory
    printf("Client Process ended\n");
    return 0;
}

void scan_buffer(int n){
    printf("Enter %d integers: ",n);
    for(int i=0;i<n;i++){
        scanf("%d",&buffer[i]);
        message.service_input[i]=buffer[i];
    }
}

void print_result(int type, int lcm, int gcd){
    if(type==1)
        printf("LCM of %d, %d, %d and %d is: %d\n",buffer[0],buffer[1],buffer[2],buffer[3],lcm);
    else if(type==2)
        printf("GCD of %d and %d is: %d\n",buffer[0],buffer[1],gcd);
    else if(type==3){
        printf("LCM of %d and %d is: %d\n",buffer[0],buffer[1],lcm);
        printf("GCD of %d and %d is: %d\n",buffer[0],buffer[1],gcd);
    }
}