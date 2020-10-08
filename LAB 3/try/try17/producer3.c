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
  int f;
  int prid;
  int cnid;
}mydata;
int *s;
typedef struct{
	pthread_mutex_t lock;
}l;
void my_handler(){}
void down(){
	if(!__sync_val_compare_and_swap(s,1,0)){
		pause();
		__sync_val_compare_and_swap(s,0,1);
	}
	// (*s)--;
}
void up(mydata *data){
	// (*s)++;
	__sync_val_compare_and_swap(s,0,1);
	kill(data->cnid,SIGUSR1);
}
void create_shared_memory(mydata *data){
	key_t keys=ftok("buffer.txt",12);
	int id=shmget(keys,sizeof(int),IPC_CREAT | 0666);
	s=(int *) shmat(id,NULL,0);
	(*s)=1;
}
int main(){
	printf("Producer Program Started...\n");
  signal(SIGUSR1,my_handler);
  int shmid, i, n;
	mydata * data;
	key_t key;
	key = ftok("buffer.txt", 11);
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
	n = 10000000;
	data->x = 0;
	data->f = 0;

	l *da;
	key_t key2=ftok("buffer.txt",15);
	int id=shmget(key2,sizeof(l),IPC_CREAT | 0666);
	da=(l *) shmat(id,NULL,0);
	pthread_mutex_init(&da->lock,NULL);
	create_shared_memory(data);
	kill(data->cnid,SIGUSR1);

	for(i=1; i<=n; i++){
		down();
		while(pthread_mutex_trylock(&da->lock)!=0);
  		(data->x)++;
		pthread_mutex_unlock(&da->lock);
		up(data);
	}

	while(data->f == 0){
  		sleep(1);
	}
	printf("Producer: Final Value of data->x = %d\n", data->x);
}
