/*THIS IS THE CONSUMER PROCESS*/
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<pthread.h>

typedef struct {
  int x;
  int f,g;
  int prid;
  int cnid;
  pthread_mutex_t lock1,lock2;
  int full,empty;
  int abc,def;
  int p,c;
}mydata;

mydata * data;

void up(int *val){
  /*data->def=0;
  if(pthread_mutex_trylock(&data->lock)==0){
  data->def=1;
  *val = *val + 1;
  pthread_mutex_unlock(&data->lock);
  }
  data->def=2;
  kill(data->prid, SIGUSR1);
  data->def=3;
  if(*val<=0)*/
    *val=*val+1;
  kill(data->cnid, SIGUSR1);
    //kill(data->cnid, SIGUSR1);
}

void down(int *val,int index){
  /*if(*val<=0){
    //if(mutex is locked)
    printf("checking, abc=%d, p=%d, c=%d  ",data->abc,data->p,data->c);
    if((data->abc==1||data->abc==0) && ((data->p) - (data->c) != 1)){
		//kill(data->prid, SIGUSR1);
      printf("pausing");
	  	pause();
	}
  }*/
  if(pthread_mutex_trylock(&data->lock2)==0)
  {
    pthread_mutex_unlock(&data->lock2);
  while(1){
  if(pthread_mutex_trylock(&data->lock1)==0){
  if(*val<=0)
	  pause();
  pthread_mutex_unlock(&data->lock1);
  break;
  }
  }
  }
  *val = *val - 1;
}


void my_handler(){}

int main(){
	printf("Producer Program Started...\n");
  signal(SIGUSR1,my_handler);
  int shmid, i, n;
	
	key_t key;
	key = ftok("buffer.txt", 28);
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
data->abc=0;
data->def=0;
data->p=0;
data->c=0;
data->g=0;
pthread_mutex_init(&data->lock1,NULL);
pthread_mutex_init(&data->lock2,NULL);
kill(data->cnid, SIGUSR1);

for(i=1; i<=n; i++){
	data->p=i;
  	down(&data->empty,i);
	printf("%d ",data->x);
	(data->x)++;
  	up(&data->full);

    
}
data->g=1;
while(data->f == 0){
  sleep(1);
}
pthread_mutex_destroy(&data->lock1);
pthread_mutex_destroy(&data->lock2);
printf("Producer: Final Value of data->x = %d\n", data->x);
}
