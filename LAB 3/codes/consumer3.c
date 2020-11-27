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
void my_handler(){}
int *s;
void down(){
	while(!__sync_val_compare_and_swap(s,1,0)){
		pause();
	}
}
void up(mydata *data){
	__sync_val_compare_and_swap(s,0,1);
	kill(data->prid,SIGUSR1);
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
  	s=(int *) shmat(id,NULL,0);
  	if(s == (void *) -1){
		perror("error2:");
		exit(1);
	}
}
int main(){
  	printf("Consumer Program Started...\n");
  	signal(SIGUSR1,my_handler);
  	int status, i, n;
  	int shmid;
  	mydata * data;
	key_t key;
	key = ftok("buffer.txt", 17);
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
  	n = 10000000;
  
  	create_shared_memory();
  	pause();

  	for(i = 1; i<=n; i++){
    	down();
    	(data->x)--;
    	up(data);
  	}
  	data->f = 1;
}
