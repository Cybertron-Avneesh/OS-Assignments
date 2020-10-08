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
  pthread_mutex_t lock;
}mydata;

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
n = 1000000;
data->x = 0;
data->f = 0;
data->g = 0;
pthread_mutex_init(&data->lock,NULL);
kill(data->cnid, SIGUSR1);

for(i=1; i<=n; i++){
	if(data->f==0){
	if(pthread_mutex_trylock(&data->lock)==0){
  	(data->x)++;
  	pthread_mutex_unlock(&data->lock);
	}
	else{
		sleep(1);
		i--;
	}
	}
	else{
	data->x++;
	}
}
data->g=1;
printf("Producer Program Completed...\n");
while(data->f == 0){
  sleep(1);
}
pthread_mutex_destroy(&data->lock);
printf("Producer: Final Value of data->x = %d\n", data->x);
}
