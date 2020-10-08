/*THIS IS THE CONSUMER PROCESS*/
/*IN THIS EXAMPLE START THE CONSUMER PROCESS FIRST*/
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<unistd.h>

typedef struct {
  int x;
  int f;
  int prid;
  int cnid;
  int value;
  int producer;
  int customer;
}mydata;

void up(mydata *S){
  S->value++;
  if(S->value<=0)
  {
	  if(S->producer==1)
	  {
		  kill(S->prid,SIGUSR1);
		  S->producer=0;
		  return ;
	  }
	  if(S->customer==1)
	  {
		  kill(S->cnid,SIGUSR1);
		  S->customer=0;
		  return ;
	  }

  }
}

void down(mydata *S){
  
  S->value--;
  if(S->value< 0)
  {
        S->customer=1;
       if(S->producer==1)
       {
           kill(S->prid,SIGUSR1);
         

       }
       else
       {
        
           pause();
       }
       
	 
  }
}

void my_handler(){}

int main(){
  printf("Consumer Program Started...\n");
  signal(SIGUSR1,my_handler);
  int status, i, n;
  int shmid;
  mydata * data;
	key_t key;
	key = ftok("buffer.txt", 16);
	if(key<0){
		perror("error0:");
		exit(1);
	}
	shmid = shmget(key, sizeof(mydata), IPC_CREAT | 0666);
  printf("%d ",shmid);
	if(shmid<0){
		perror("error1:");
		exit(1);
	}
	data = (mydata *) shmat(shmid, NULL, 0);
	if(data == (void *) -1){
		perror("error2:");
		exit(1);
	}

  data->cnid = getpid();
  n = 1000;
  pause();

  for(i = 1; i<=n; i++){
  
    down(data);
    printf("%d ",i);
    (data->x)--;
    up(data);
  }
  kill(data->prid, SIGUSR1);
}