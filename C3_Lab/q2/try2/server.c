#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int gcd(int a, int b){ 
    if (b == 0) 
        return a; 
    return gcd(b, a % b); 
}
int lcm(int arg[]){
	int lcm=arg[0];
    for(int i=1;i<4;i++){
        lcm=((arg[i]*lcm)/gcd(arg[i],lcm));
    }
}
int main(int argc,char * argv[]){
    if(argc<2){
        fprintf(stderr, "Please provide the port number");
        exit(1);
    }
    int sockfd,newsockfd,portno,n,ans=1;
    char buffer[1024];
    pid_t childpid;
    struct sockaddr_in server_address,client_address;
    socklen_t client_len;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("Error in creating a socket");
        exit(1);
    }
    bzero((char *) &server_address,sizeof(server_address));
    portno=atoi(argv[1]);
    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=INADDR_ANY;
    server_address.sin_port=htons(portno);
    int tr=1;
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int))==-1){
        perror("Not a useable port");
        exit(1);
    }
    if(bind(sockfd,(struct sockaddr *) &server_address,sizeof(server_address))<0){
        perror("Not able to bind");
        exit(1);
    }
    listen(sockfd,5);
    client_len=sizeof(client_address);
	while(1){
		newsockfd = accept(sockfd, (struct sockaddr*)&client_address, &client_len);
		if(newsockfd < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
		if((childpid = fork()) == 0){
			close(sockfd);
            bzero(buffer,1024);
            printf("\tNow taking input from client\n");
            int arg[4];
            n=read(newsockfd,arg,sizeof(arg));
            if(n<0){
                perror("Error while reading");
                exit(1);
            }
            ans=lcm(arg);
            n=write(newsockfd,&ans,sizeof(int));
            if(n<0){
                perror("Error while writing");
                exit(1);
            }
		}

	}
    close(sockfd);
	close(newsockfd);
	return 0;
}
