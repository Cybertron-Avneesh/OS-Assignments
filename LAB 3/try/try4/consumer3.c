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
}mydata;
typedef struct {
	int s;
	int pid;
}shared;

// void create_shared(){
  
// }
void up(int *val,mydata *data){
  *val = *val + 1;
  while(sh->pid!=data->prid);
  kill(data->prid,SIGUSR1);
}

void down(int *val,mydata *data){
    printf("-100\n");
  if(*val<=0){
    sh->pid=data->cnid;
    pause();
  }
  *val = *val - 1;
}

void my_handler(){}

int main(){
  printf("Consumer Program Started...\n");
  signal(SIGUSR1,my_handler);
  int status, i, n;
  int shmid;
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

  data->cnid = getpid();
  n = 1000000;
  pause();

  shared *sh;
  int shmid1;
  key_t key1;
  key1=ftok("buffer.txt",13);
  shmid1=shmget(key1,sizeof(shared),IPC_CREAT|0666);
  sh=(shared*)shmat(shmid1,NULL,0);
  pause();
  for(i = 1; i<=n; i++){
    down(&sh->s,data);
    // printf("%d ",data->x);
    (data->x)--;
    up(&sh->s,data);
  }
  data->f = 1;
}
