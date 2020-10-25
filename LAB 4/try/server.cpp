#include<bits/stdc++.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<unistd.h>

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

int main(){
  //printf("Hello, I am Server Program.   My PID is %d\n", getpid());
  create_shared_memory();
  data->result_size=0;
  while(1){
      if(data->result_size>0){
          struct service_choice service=data->request_queue.front();
          int pid = fork();
          if(pid == 0){
            if(service.serve_type == 1){
                char charbuffer[4][100000];
                sprintf(charbuffer[0], "%d", service.buffer[0]);
                sprintf(charbuffer[1], "%d", service.buffer[1]);
                sprintf(charbuffer[2], "%d", service.buffer[2]);
                sprintf(charbuffer[3], "%d", service.buffer[3]);
                execl("./service1", "./service1", charbuffer[0], charbuffer[1], charbuffer[2], charbuffer[3], service.pid, NULL);
            }
            else if(service.serve_type == 2){
                char charbuffer[2][100000];
                sprintf(charbuffer[0], "%d", service.buffer[0]);
                sprintf(charbuffer[1], "%d", service.buffer[1]);
                execl("./service2", "./service2", charbuffer[0], charbuffer[1], service.pid, NULL);
            }
            else{
                printf("You have entered invalid choice.\n");
                exit(0);
            }
          }
          data->request_queue.pop();
      }
      else{
          sleep(1);
      }
  }
  printf("Server Program Exiting...\n");
  return 0;
}
