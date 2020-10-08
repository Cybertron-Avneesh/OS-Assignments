/*THIS IS THE CONSUMER PROCESS*/
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
      
           S->producer=1;
       if(S->customer==1)
       {
           kill(S->cnid,SIGUSR1);
         

       }
       else
       {
           pause();
       }
  }
}

void my_handler(){}

int main(){
	printf("Producer Program Started...\n");
  signal(SIGUSR1,my_handler);
  int shmid, i, n;
	mydata * data;
	key_t key;
	key = ftok("buffer.txt", 16);
	if(key<0){
		perror("error0:");
		exit(1);
	}
	shmid = shmget(key, sizeof(mydata), IPC_CREAT | 0666);
	if(shmid<0){
		perror("error1:");
		exit(1);
	}
	data = (mydata *) shmat(shmid, NULL, 0);
	if(data == (void *) -1){
		perror("error2:");
		exit(1);
	}

data->prid = getpid();
n =  1000;
data->x = 0;
data->f = 0;
data->value=1;
data->producer=0;
data->customer=0;
kill(data->cnid, SIGUSR1);

for(i=1; i<=n; i++){
	
  	down(data);
	printf("%d ",i);
	(data->x)++;
  	up(data);
}

pause();
printf("Producer: Final Value of data->x = %d\n", data->x);
}