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
  pthread_mutex_t lock;
  int full,empty;
}mydata;

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
    //printf("%d, ",i);
    if(data->g==0){
    if(pthread_mutex_trylock(&data->lock)==0){
    //printf("%d ",data->x);
    (data->x)--;
    printf("unlock- %d ", pthread_mutex_unlock(&data->lock));
    kill(data->prid, SIGUSR1);
    }
    else{
      //sleep(1);
      i--;
      pause();
    }
    
    }
    else
    {
      data->x--;
    }
    
  }
  data->f = 1;
}