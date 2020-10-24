#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>

struct msg_buffer
{
    long msg_type;
    int service_no;
    int PID;
    int service_input[4];
} message;

int msg_id;

void my_handler(){
    //Clearing request queue shared memory
    msgctl(msg_id, IPC_RMID, NULL);
    exit(1);
}

int main()
{
    signal(SIGINT, my_handler);
    key_t my_key;
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
            int key = ftok("buffer.txt", message.PID);
            int shmid = shmget(key, 2 * sizeof(int), IPC_CREAT | 0666);
            int *result_buffer = (int *)shmat(shmid, NULL, 0);
            char argv_1[50];
            char argv_2[50];
            char argv_5[50];
            sprintf(argv_1, "%d", message.service_input[0]);
            sprintf(argv_2, "%d", message.service_input[1]);
            sprintf(argv_5, "%d", message.PID);

            if(message.service_no==2)
                execl("./service2", "./service2", argv_1, argv_2, argv_5, NULL);
            else if(message.service_no==3)
                execl("./service3", "./service3", argv_1, argv_2, argv_5, NULL);
            else if(message.service_no==1){ 
                char argv_3[50];
                char argv_4[50];
                sprintf(argv_3, "%d", message.service_input[2]);
                sprintf(argv_4, "%d", message.service_input[3]);
                execl("./service1", "./service1", argv_1, argv_2, argv_3, argv_4, argv_5, NULL);
            }
            else{
                printf("Invalid Option\n");
            }
            
        }
        wait( NULL );
    }
    return 0;
}