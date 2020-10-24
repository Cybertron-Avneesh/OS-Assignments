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
typedef struct{
    int result[2];
}result_buffer;
int gcd(int a, int b){ 
    if (b == 0)
        return a; 
    return gcd(b, a % b); 
}
void Sigusr1_handler(){}
int main(int argc,char * argv[]){
    signal(SIGUSR1,Sigusr1_handler);
    int client_process_id=atoi(argv[1]);
    key_t key_result_buffer=atoi(argv[2]);
    int input_arguments_length=atoi(argv[3]);
    int input_array[4];
    for(int i=1;i<=atoi(argv[3]);i++){
        input_array[i-1]=atoi(argv[3+i]);
    }
    result_buffer * result_buf;
    int shmid=shmget(key_result_buffer,sizeof(result_buffer),IPC_CREAT | 0666);
    if(shmid<0){
		perror("error1:");
		exit(1);
	}
    result_buf=(result_buffer *) shmat(shmid,NULL,0);
    if(result_buf == (void *) -1){
		perror("error2:");
		exit(1);
	}
    result_buf->result[0]=gcd(input_array[0],input_array[1]);
    kill(client_process_id,SIGUSR1);
}