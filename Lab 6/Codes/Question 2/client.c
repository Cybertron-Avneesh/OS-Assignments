#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<netdb.h>
int main(int argc,char * argv[]){
    int sockfd,portno,n;
    struct sockaddr_in server_address;
    struct hostent *server;
    char buffer[255];
    if(argc<3){
        fprintf(stderr,"usage %s hostname port\n",argv[0]);
        exit(1);
    }
    portno=atoi(argv[2]);
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("Error in creating a socket");
        exit(1);
    }
    server=gethostbyname(argv[1]);
    if(server==NULL){
        fprintf(stderr,"Error, no such host");
        exit(1);
    }
    bzero((char *) &server_address,sizeof(server_address));
    server_address.sin_family=AF_INET;
    bcopy((char *) server->h_addr_list[0],&server_address.sin_addr.s_addr,server->h_length);
    server_address.sin_port=htons(portno);
    if(connect(sockfd,(struct sockaddr *) &server_address,sizeof(server_address))<0){
        perror("Connection Failed");
        exit(1);
    }
    printf("currently 1 service is available-LCM of 4 Numbers\n");
	printf("\tenter the 4 numbers: ");
    int arg[4];
    for(int i=0;i<4;i++){
        scanf("%d",&arg[i]);
    }
    n=write(sockfd,arg,sizeof(arg));
    if(n<0){
        perror("Error while writing");
        exit(1);
    }
    int ans;
    n=read(sockfd,&ans,sizeof(int));
    if(n<0){
        perror("Error while reading");
        exit(1);
    }
    printf("Server: LCM is %d\n",ans);
	return 0;
}