#include<stdio.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>

#define MAX 50
#define PORT 41234

void write_read(int sockfd)
{
	char buff[MAX];
	int n;
	pid_t pid;
	pid=fork();
	if(pid>0)
	{
	 while(1)
	 {
		bzero(buff,sizeof(buff));
		printf("\tMe: ");
		n=0;
		while((buff[n++]=getchar())!='\n');
		send(sockfd,buff,sizeof(buff),0);
		if((strncmp(buff,"exit",4))==0)
		{
		  printf("Client Disconnected\n");
		  break;
		}
	 }
	}
	else if(pid==0)
	{
	 while(1)
	 {
	  bzero(buff,sizeof(buff));
	  recv(sockfd,buff,sizeof(buff),0);
	  printf("\nServer : %s",buff);
	  if((strncmp(buff,"exit",4))==0)
	  {
		printf("Client Disconnected\n");
		break;
	  }
	 }
	}
	else
        {
	   printf("Fork error");
        }
}
int main()
{
    int sockfd,connfd;
    struct sockaddr_in servaddr,cli;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)
	{
	  printf("socket creation failed...\n");
	  exit(0);
	}
    else
	printf("Socket successfully created..\n");
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=inet_addr("127.0.0.1"); 
    servaddr.sin_port=htons(PORT);
    if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)
    {
	printf("connection with the server failed...\n");
	exit(0);
    } 
    else
	printf("connected to the server..\n");
    write_read(sockfd);
    close(sockfd);
}
