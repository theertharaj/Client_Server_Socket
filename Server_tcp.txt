#include<stdio.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>

#define MAX 50
#define PORT 41234

void read_write(int sockfd)
{
   char buff[MAX];
   int n;
   pid_t pid;
   pid=fork();
   if(pid>0)
   {
	while(1)
	{
	  bzero(buff,MAX);
	  recv(sockfd,buff,sizeof(buff),0);
	  printf("\nclient %d: %s \n \t",sockfd,buff);
	  if(strncmp("exit",buff,4)==0)
	  {
		printf("Server disconnected\n");
		break;
	  }
	}
   }
   else if(pid==0)
   {
	while(1)
	{
	   bzero(buff,MAX);
	   n=0;
	   while((buff[n++]=getchar())!='\n');
	   send(sockfd,buff,sizeof(buff),0);
	   if((strncmp("exit",buff,4))==0)
	   {
		printf("Server Disconnected\n");
		break;
	   }
	} 
   }
   else
   {
	printf("Fork Error!!");
   }
}

int main()
{
  int sockfd,connfd,len;
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
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  servaddr.sin_port=htons(PORT);
  if((bind(sockfd,(struct sockaddr *)&servaddr, sizeof(servaddr)))!=0)
  {
	printf("socket bind failed...\n");
	exit(0);
  }
  else
	printf("Socket successfully binded..\n");

  if((listen(sockfd,5))!=0)
  {
	printf("Listen failed...\n");
	exit(0);
  }
  else
	printf("Server listening........\n");

  len=sizeof(cli);
  connfd=accept(sockfd,(struct sockaddr *)&cli,&len);
  if(connfd<0)
  {
	printf("server acccept failed...\n");
	exit(0);
  }
  else
	printf("server acccepted the client...\n");
  read_write(connfd);
  close(sockfd);
}
