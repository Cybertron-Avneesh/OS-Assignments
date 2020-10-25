#include<bits/stdc++.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/shm.h>

using namespace std;

struct service_choice{
    int pid;
    int serve_type;
    int buffer[4];
}s;

struct shared_memory{
    queue <service_choice> request_queue;
    int result_buffer[100000][3];
    int result_size;
    //int lock;
}*data;

void create_shared_memory(){
  	key_t keys=ftok("buffer.txt",18);
	if(keys<0){
		perror("error0:");
		exit(1);
	}
  	int id=shmget(keys,sizeof(int),IPC_CREAT | 0666);
	if(id<0){
		perror("error1:");
		exit(1);
	}
  	data=(shared_memory *) shmat(id,NULL,0);
  	if(data == (void *) -1){
		perror("error2:");
		exit(1);
	}
}


int main(int argc, char * argv[]){
	create_shared_memory();
  printf("Hello, I am Service1 prog.   My PID is %d\n", getpid());
  int answer=0;
  for(int i=1; i<=4; i++){
    answer+=atoi(argv[i]);
  }
	//lock
	data->result_buffer[data->result_size][0]=*argv[5];
	data->result_buffer[data->result_size][1]=answer;
	data->result_size++;
	//unlock
  printf("Service1 is exiting...\n");
  return 0;
}
