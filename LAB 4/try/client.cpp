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

void scan_buffer(int n){
    printf("Enter %d",n);
    printf(" integers: ");
    for(int i=0;i<n;i++)
        scanf("%d",&s.buffer[i]);
}

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
  int service_type=0;
  while(service_type<1 || service_type>3){
      printf("Enter service type: \n");
      printf("1-> LCM of 4 integers\n");
      printf("2-> GCD of 2 integers\n");
      printf("3-> LCM and GCD of 2 integers\n");
      scanf("%d",&service_type);
  }
  s.pid=getpid();
  s.serve_type=service_type;
  for(int i=0;i<4;i++)
    s.buffer[i]=-1;
  switch(service_type){
        case 1:     scan_buffer(4);
                    break;
        case 2:     
        case 3:     scan_buffer(2);
                    break;
        default:    printf("Invalid choice\n");
                    exit(-1);
  }
  (data->request_queue).push(s);
  pause();
  int index;
  for(int i=0;i<data->result_size;i++){
      if(data->result_buffer[i][0]==getpid()){
        //print the result
        printf("Result=%d\n",data->result_buffer[i][1]);
        index=i;
        break;
      }
  }
  //lock(mutex)
    for(int i=index;i<data->result_size;i++)
        for(int j=0;j<3;j++){
            data->result_buffer[i][j]=data->result_buffer[i+1][j];
        }

    data->result_size--;
  //unlock(mutex)
  

  printf("Client Program Exiting...\n");
  return 0;
}