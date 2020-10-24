#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
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
queue *q;
mydata *data;
void down(){
	while(!__sync_val_compare_and_swap(&(data->s),1,0)){
		pause();
	}
}
void up(){
	__sync_val_compare_and_swap(&(data->s),0,1);
	kill(data->client_process_id,SIGUSR2);
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
int length;
void pop(char *service_arguments[5+length]){
    if(data->top<0){
        return;
    }
    int process_id=q->qu[data->top].process_id;
    key_t key_result_buffer=q->qu[data->top].key_result_buffer;
    int input_arguments_length=q->qu[data->top].input_arguments_length;
    int input_array[4];
    for(int i=0;i<input_arguments_length;i++){
        input_array[i]=q->qu[data->top].input_array[i];
    }
    char buffer1[1000];
    char *service_type=q->qu[data->top].service_type;
    if((service_type)=="service1"){
        service_arguments[0]="./service1";
    }
    else if((service_type)=="service2"){
        service_arguments[0]="./service2";
    }
    else if((service_type)=="service3"){
        service_arguments[0]="./service3";
    }
    sprintf(buffer1,"%d",process_id);
    service_arguments[1]=buffer1;
    sprintf(service_arguments[2],"%d",key_result_buffer);
    sprintf(service_arguments[3],"%d",input_arguments_length);
    sprintf(service_arguments[4],"%d",input_array[0]);
    sprintf(service_arguments[5],"%d",input_array[1]);
    if(input_arguments_length==4){
        sprintf(service_arguments[6],"%d",input_array[2]);
        sprintf(service_arguments[7],"%d",input_array[3]);
    }
    char * buffer2="NULL";
    service_arguments[8]=buffer2;
    if(data->count==1){
        data->top=-1;
        data->count=0;
    }
    else{
        data->count--;
    }
    return;
}
void push(int process_id,key_t key_result_buffer,int input_array[],int input_arguments_length,char *service_type){
    data->top++;
    q->qu[data->top].process_id=process_id;
    q->qu[data->top].key_result_buffer=key_result_buffer;
    for(int i=0;i<input_arguments_length;i++){
        q->qu[data->top].input_array[i]=input_array[i];
    }
    q->qu[data->top].input_arguments_length=input_arguments_length;
    q->qu[data->top].service_type=service_type;
}
void Sigusr2_handler(){}
int main(){
    create_shared_memory();
    signal(SIGUSR2,Sigusr2_handler);
    data->s=1;
    data->top=-1;
    data->count=0;
    data->server_process_id=getpid();
    // pause();
    while (1){
        while(data->count!=0){
            printf("queue not empty");
            down();
            length=q->qu[data->top].input_arguments_length;
            char * service_arguments[5+length];
            char *service_type=q->qu[data->top].service_type;
            pop(service_arguments);
            up();
            int pid=fork();
            if(pid==0){
                if((service_type)=="service1"){
                    printf("Going to service 1\n");
                    execv("./service1",service_arguments);
                }
                else if((service_type)=="service2"){
                    printf("Going to service 2\n");
                    execv("./service2",service_arguments);
                }
                else if((service_type)=="service3"){
                    printf("Going to service 3\n");
                    execv("./service3",service_arguments);
                }
            }
        }
    }
}