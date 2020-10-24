#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char * argv[]){
  printf("Hello, I am Server Program.   My PID is %d\n", getpid());
  int ch, pid, n, c;
  char buffer1[10];
  char buffer2[10];
  while(1){
      printf("Enter value of n and c:  ");
      scanf("%d %d", &n, &c);
      sprintf(buffer1, "%d", n);
      sprintf(buffer2, "%d", c);
      printf("Enter Your Choice (0/1), Press -1 to exit: \n");
      scanf("%d", &ch);
      if(ch == -1) break;
      pid = fork();
      if(pid == 0){
        if(ch == 0){
          execl("./service1", "./service1", buffer1, buffer2, NULL);
        }
        else if(ch == 1){
          execl("./service2", "./service2", buffer1, buffer2, NULL);
        }
        else{
          printf("You have entered invalid choice.\n");
          exit(0);
        }
      }
      //wait(NULL);
  }
  printf("Server Program Exiting...\n");
  return 0;
}
