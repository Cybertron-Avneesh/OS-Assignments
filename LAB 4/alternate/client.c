#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

#include <signal.h>
#include <sys/shm.h>

// structure for message queue
struct request
{
    int service_no;
    int PID;
    //  int service_input[4];
    int input_1;
    int input_2;
    int input_3;
    int input_4;

    // int *result_buffer;
};
// Define message queue structure
struct msg_buffer
{
    long msg_type;
    struct request a;

} message;
void my_handler(){}
int main()
{
    key_t my_key;
    int msg_id;
    signal(SIGUSR1, my_handler);
    my_key = ftok("progfile", 66);             //create unique key
    msg_id = msgget(my_key, 0666 | IPC_CREAT); //create message queue and return id
    message.msg_type = 1;
    printf("Enter the service Number : ");
    scanf("%d", &(message.a.service_no));

    printf("Enter first  Number : ");
    scanf("%d", &(message.a.input_1));

    printf("Enter second Number : ");
    scanf("%d", &(message.a.input_2));

    printf("Enter third number : ");
    scanf("%d", &(message.a.input_3));

    printf("Enter fourth Number : ");
    scanf("%d", &(message.a.input_4));

    message.a.PID = getpid();
    int key = ftok("buffer.txt", getpid());
    int shmid = shmget(key, 2 * sizeof(int), IPC_CREAT | 0666);
    int *result_buffer = (int *)shmat(shmid, NULL, 0);

    //scanf('\0');
    //  scanf("%d ",&message.a.result_buffer[0]);

    //fgets(message.msg, 100, stdin);
    msgsnd(msg_id, &message, sizeof(message), 0); //send message
    pause();
   
    printf("GCD is : %d \n", result_buffer[0]);

    return 0;
}