#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
typedef struct{
    int process_id;
    key_t key_result_buffer;
    int input_array[4];
    int input_arguments_length;
    char *service_type;
}que;
typedef struct{
    que qu[10000];    
}queue;
typedef struct{
    int s;
    int top;
    int count;
    int server_process_id;
    int client_process_id;
}mydata;
typedef struct{
    int result[2];
}result_buffer;
queue *q;
mydata *data;
void down(){
	while(!__sync_val_compare_and_swap(&(data->s),1,0)){
		pause();
	}
}
void up(){
	__sync_val_compare_and_swap(&(data->s),0,1);
	kill(data->server_process_id,SIGUSR2);
}
void create_shared_memory(){
    key_t key_queue=ftok("buffer.txt",50);
    if(key_queue<0){
		perror("error0:");
		exit(1);
	}
	int shmid_queue=shmget(key_queue,sizeof(queue),IPC_CREAT | 0666);
    if(shmid_queue<0){
		perror("error1:");
		exit(1);
	}
	q=(queue *) shmat(shmid_queue,NULL,0);
    if(q == (void *) -1){
		perror("error2:");
		exit(1);
	}
    key_t key_mydata=ftok("buffer.txt",51);
    if(key_mydata<0){
		perror("error0:");
		exit(1);
	}
	int shmid_mydata=shmget(key_mydata,sizeof(mydata),IPC_CREAT | 0666);
    if(shmid_mydata<0){
		perror("error1:");
		exit(1);
	}
	data=(mydata *) shmat(shmid_mydata,NULL,0);
    if(data == (void *) -1){
		perror("error2:");
		exit(1);
	}
}
void push(int process_id,key_t key_result_buffer,int input_array[],int input_arguments_length,char *service_type){
    data->top++;
    data->count++;
    q->qu[data->top].process_id=process_id;
    q->qu[data->top].key_result_buffer=key_result_buffer;
    for(int i=0;i<input_arguments_length;i++){
        q->qu[data->top].input_array[i]=input_array[i];
    }
    q->qu[data->top].input_arguments_length=input_arguments_length;
    q->qu[data->top].service_type=service_type;
    return;
}
void Sigusr1_handler(){}
void Sigusr2_handler(){}
int main(){
    signal(SIGUSR1,Sigusr1_handler);
    signal(SIGUSR2,Sigusr2_handler);
    create_shared_memory();
    data->client_process_id=getpid();
    // kill(data->server_process_id,SIGUSR2);
    printf("Hi! I am client process\n");
    printf("For\nService 1: LCM of 4 numbers (press 1)\nService 2: GCD of 2 numbers (press 2)\nService 3: GCD and LCM of 2 numbers (press 3)\n");
    printf("Enter your choice: ");
    int choice;
    scanf("%d",&choice);
    if(choice==1){
        printf("Please enter the 4 numbers:");
        int input_array[4]={0};
        for(int i=0;i<4;i++){
            scanf("%d",&input_array[i]);
        }
        result_buffer *result_buf;
        key_t key_result_buffer=ftok("buffer.txt",52);
        if(key_result_buffer<0){
		    perror("error0:");
		    exit(1);
	    }
	    int shmid_result_buffer=shmget(key_result_buffer,sizeof(result_buffer),IPC_CREAT | 0666);
        if(shmid_result_buffer<0){
	    	perror("error1:");
		    exit(1);
	    }
	    result_buf=(result_buffer *) shmat(shmid_result_buffer,NULL,0);
        if(result_buf == (void *) -1){
		    perror("error2:");
		    exit(1);
	    }
        push(getpid(),key_result_buffer,input_array,4,"service1");
        int pid=fork();
        if(pid==0){
            printf("requesting server");
            execl("./server","./server",NULL);
        }
        wait(NULL);
        int ans=(result_buf->result[0]);
        printf("LCM of 4 numbers entered is: %d",ans);       
    }
    else if(choice==3){
        printf("Please enter the 2 numbers:");
        int input_array[4]={0};
        for(int i=0;i<2;i++){
            scanf("%d",&input_array[i]);
        }
        result_buffer *result_buf;
        key_t key_result_buffer=ftok("buffer.txt",52);
        if(key_result_buffer<0){
		    perror("error0:");
		    exit(1);
	    }
	    int shmid_result_buffer=shmget(key_result_buffer,sizeof(result_buffer),IPC_CREAT | 0666);
        if(shmid_result_buffer<0){
	    	perror("error1:");
		    exit(1);
	    }
	    result_buf=(result_buffer *) shmat(shmid_result_buffer,NULL,0);
        if(result_buf == (void *) -1){
		    perror("error2:");
		    exit(1);
	    }
        push(getpid(),key_result_buffer,input_array,2,"service3");
        int pid=fork();
        if(pid==0){
            printf("requesting server");
            execl("./server","./server",NULL);
        }
        wait(NULL);
        int ans=(result_buf->result[0]);
        printf("GCD of 2 numbers entered is: %d",ans);
    }
    else if(choice==2){
        printf("choice 2 selected\n3\n");
        printf("Please enter the 2 numbers:\n");
        int *input_array;
        input_array=(int *)malloc(4*sizeof(int));
        printf("Enter the first number: \n");
        int a,b;
        scanf("%d",&a);
        printf("Enter the second number: \n");
        scanf("%d",&b);
        printf("\n%d",a);
        result_buffer *result_buf;
        key_t key_result_buffer=ftok("buffer.txt",54);
        if(key_result_buffer<0){
		    perror("error0:");
		    exit(1);
	    }
	    int shmid_result_buffer=shmget(key_result_buffer,sizeof(result_buffer),IPC_CREAT | 0666);
        if(shmid_result_buffer<0){
	    	perror("error1:");
		    exit(1);
	    }
	    result_buf=(result_buffer *) shmat(shmid_result_buffer,NULL,0);
        if(result_buf == (void *) -1){
		    perror("error2:");
		    exit(1);
	    }
        down();
        push(getpid(),key_result_buffer,input_array,2,"service2");
        up();
        // pid_t pid=fork();
        // if(pid < 0){
        //     perror("Fork Error.");
        //     exit(1qu[data->top);
        // }
        // if(pid==0){
        //     printf("requesting server");
        //     // execl("./server","./server",NULL);
        // }
        // else{
            pause();
            int lcm=(result_buf->result[0]);
            int gcd=(result_buf->result[1]);
            printf("LCM and GCD of 2 numbers entered is: %d %d",lcm,gcd);
        // }
        
    }
}