/*THIS IS THE CONSUMER PROCESS*/
/*IN THIS EXAMPLE START THE CONSUMER PROCESS FIRST*/
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
  pthread_mutex_t lock1,lock2,lock;
  int full,empty;
  int abc,def;
  int p,c;
}mydata;

mydata * data;

void up(int *val){
  /*data->abc=0;
  if(pthread_mutex_trylock(&data->lock)==0){
  
  *val = *val + 1;
  pthread_mutex_unlock(&data->lock);
  }
  data->abc=2;
  kill(data->prid, SIGUSR1);
  data->abc=3;
  if(*val<=0)*/
      kill(data->prid, SIGUSR1);
    *val=*val+1;
    //kill(data->prid, SIGUSR1);
}

void down(int *val,int index){
  /*if(*val<=0){
    //if(mutex is locked)
    printf("checking");
    if((data->def==1 || data->def==0) && ((data->p) - (data->c) != 0)){
      kill(data->prid, SIGUSR1);
      printf("pausing");*/
      //if(index!=1)
    if(pthread_mutex_trylock(&data->lock1)==0)
    {
      pthread_mutex_unlock(&data->lock1);
      while(1){
      if(pthread_mutex_trylock(&data->lock2)==0){
      if(*val<=0)
	      pause();
      pthread_mutex_unlock(&data->lock2);
      //data->abc=1;
      break;
      }
      }
    }
  *val = *val - 1;
}

void my_handler(){}

int main(){
  printf("Consumer Program Started...\n");
  signal(SIGUSR1,my_handler);
  int status, i, n;
  int shmid;
	key_t key;
	key = ftok("buffer.txt", 27);
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

  data->cnid = getpid();
  n = 1000000;
  pause();

  for(i = 1; i<=n; i++){
    data->c=i;
    if(data->g==0){
    down(&data->full,i);
    if(pthread_mutex_trylock(&data->lock)==0){
    //printf("%d ",data->x);
    (data->x)--;
    pthread_mutex_unlock(&data->lock);
    //kill(data->prid, SIGUSR1);
    }
    else{
      //sleep(1);
      i--;
      //pause();
    }
    up(&data->empty);
    }
    else
    {
      data->x--;
    }

    
  }
  data->f = 1;
}