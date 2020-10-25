#include<bits/stdc++.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/shm.h>

using namespace std;

int main(int argc, char * argv[]){
  printf("Hello, I am Service2 prog.   My PID is %d\n", getpid());
  int i, n, c;
  /* printf("My arguments are:-\n");
  for(i=0; i<argc; i++){
    printf("%s\n", argv[i]);
  }*/
  n = atoi(argv[1]);
  c = atoi(argv[2]);
  for(i=0; i<c; i++){
    printf("Service2 : n = %d\n", n--);
    sleep(1);
  }

  printf("Service2 is exiting...\n");
  return 0;
}
