#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>

// Define message queue structure
struct msg_buffer
{
    long msg_type;
    int service_no;
    int PID;
    int service_input[4];
} message;

int buffer[4];
void scan_buffer(int n);
void print_result(int type, int lcm, int gcd);

void my_handler(){}
int main()
{
    printf("PID of this client process is: %d\n",getpid());
    key_t my_key;
    int msg_id;
    signal(SIGUSR1, my_handler);
    my_key = ftok("progfile", 66);             //create unique key
    msg_id = msgget(my_key, 0666 | IPC_CREAT); //create message queue and return id
    message.msg_type = 1;
    int service_type=0;
    while(service_type<1 || service_type>3){
      printf("Enter service type: \n");
      printf("1-> LCM of 4 integers\n");
      printf("2-> GCD of 2 integers\n");
      printf("3-> LCM and GCD of 2 integers\n");
      scanf("%d",&service_type);
    }
    message.service_no=service_type;
    switch(service_type){
        case 1:     scan_buffer(4);
                    break;
        case 2:     
        case 3:     scan_buffer(2);
                    break;
        default:    printf("Invalid choice\n");
                    exit(-1);
    }

    message.PID = getpid();
    int key = ftok("buffer.txt", getpid());
    int shmid = shmget(key, 2 * sizeof(int), IPC_CREAT | 0666);
    int *result_buffer = (int *)shmat(shmid, NULL, 0);

    msgsnd(msg_id, &message, sizeof(message), 0); //send message
    pause();
    switch(service_type){
        case 1:     print_result(1,result_buffer[0],0);
                    break;
        case 2:     print_result(2,0,result_buffer[0]);
                    break;
        case 3:     print_result(3,result_buffer[0],result_buffer[1]);
                    break;
        default:    printf("Invalid choice\n");
                    exit(-1);
    }

    shmctl(shmid, IPC_RMID, NULL);  //free result_buffer from memory
    return 0;
}

void scan_buffer(int n){
    printf("Enter %d",n);
    printf(" integers: ");
    for(int i=0;i<n;i++){
        scanf("%d",&buffer[i]);
        message.service_input[i]=buffer[i];
    }
}

void print_result(int type, int lcm, int gcd){
    if(type==1)
        printf("LCM of %d, %d, %d and %d is: %d\n",buffer[0],buffer[1],buffer[2],buffer[3],lcm);
    else if(type==2)
        printf("GCD of %d and %d is: %d\n",buffer[0],buffer[1],gcd);
    else if(type==3){
        printf("LCM of %d and %d is: %d\n",buffer[0],buffer[1],lcm);
        printf("GCD of %d and %d is: %d\n",buffer[0],buffer[1],gcd);
    }
}