#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
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
int main()
{
    key_t my_key;
    int msg_id;
    my_key = ftok("progfile", 66); //create unique key
    msg_id = msgget(my_key, 0666 | IPC_CREAT);
    msgctl(msg_id, IPC_RMID, NULL);
    msg_id = msgget(my_key, 0666 | IPC_CREAT); //create message queue and return id
    while (1)
    {
        int pid = fork();
        if (pid < 0)
        {
            perror("Fork Error.");
            exit(1);
        }
        else if (pid == 0)
        {
            msgrcv(msg_id, &message, sizeof(message), 1, 0); //used to receive message
            // display the message
            //      printf("Service number %d\n", message.a.service_no);
            //    printf("Process ID %d\n", message.a.PID);
            int key = ftok("buffer.txt", message.a.PID);
            int shmid = shmget(key, 2 * sizeof(int), IPC_CREAT | 0666);
            int *result_buffer = (int *)shmat(shmid, NULL, 0);
            //  printf("%d\n", result_buffer[0]);
            char argv_1[50];
            char argv_2[50];
            char argv_3[50];
            char argv_4[50];
            char argv_5[50];
            sprintf(argv_1, "%d", message.a.input_1);
            sprintf(argv_2, "%d", message.a.input_2);
            sprintf(argv_3, "%d", message.a.input_3);
            sprintf(argv_4, "%d", message.a.input_4);
            sprintf(argv_5, "%d", message.a.PID);
            execl("./service1", "./service1", argv_1, argv_2, argv_3, argv_4, argv_5, NULL);
        }
        wait( NULL );
    }
    msgctl(msg_id, IPC_RMID, NULL); //destroy the message queue
    return 0;
}