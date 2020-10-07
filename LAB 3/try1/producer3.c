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
  int full,empty;
}mydata;


void up(int *val,mydata *data){
  *val = *val + 1;
  kill(data->cnid,SIGUSR1);
}

void down(int *val){
  if(*val<=0){
	  pause();
  }
  *val = *val - 1;
}

void my_handler(){}

int main(){
	printf("Producer Program Started...\n");
  signal(SIGUSR1,my_handler);
  int shmid, i, n;
	mydata * data;
	key_t key;
	key = ftok("buffer.txt", 12);
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
n = 1000000;
data->x = 0;
data->f = 0;
data->full=0;
data->empty=1;
kill(data->cnid, SIGUSR1);

for(i=1; i<=n; i++){
  	down(&data->empty);
	// printf("%d ",data->x);
	(data->x)++;
  	up(&data->full,data);
}

while(data->f == 0){
  sleep(1);
}
printf("Producer: Final Value of data->x = %d\n", data->x);
}
