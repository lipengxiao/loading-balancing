#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<stdbool.h>
#include<stdlib.h>

#define BUFF 128

int main(int argc,char*argv[])
{
	if(argc < 3 || argc > 3)
	{
		printf("err argv!\n");
		exit(0);
	}
	char*ip = argv[1];
	int port = atoi(argv[2]);
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
	{
		printf("socket err\n");
		exit(0);
	}
	struct sockaddr_in saddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);
	int c_fd = connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
	if(c_fd == -1)
	{
		printf("connect server err\r\n");
		close(sockfd);
		exit(0);
	}
	int dispatch_fd = socket(AF_INET,SOCK_STREAM,0);
	if(dispatch_fd == -1)
	{
		printf("dispatch_fd create err.\n");
		fflush(stdout);
		close(sockfd);
		exit(0);
	}
	listen(dispatch_fd,5);
	struct sockaddr_in caddr;
	int len = sizeof(caddr);
	int res; 
	res	= getsockname(dispatch_fd,(struct sockaddr*)&caddr,&len);
	if(res == -1)
	{
		printf("getsockname err\n");
		close(dispatch_fd);
		close(sockfd);
		exit(0);
	}
	printf("ip:%s,port:%d\n",inet_ntoa(caddr.sin_addr),ntohs(caddr.sin_port));
	fflush(stdout);
	int client_fd;
	struct sockaddr_in addr;
	socklen_t length = sizeof(addr);
	char buff[BUFF] = "";
	while(1)
	{
		client_fd = accept(dispatch_fd,(struct sockaddr*)&addr,&length);
		if(client_fd == -1)
		{
			printf("accept err\n");
			close(dispatch_fd);
			close(sockfd);
			exit(0);
		}
		while(1)
		{
			res = recv(client_fd,buff,BUFF-1,0);
			if(res == 0)
			{
				printf("client %d over!\n",c_fd);
				break;
			}
			printf("buff:%s\n",buff);
			fflush(stdout);
			send(sockfd,buff,strlen(buff),0);
			memset(buff,0,sizeof(buff));
		}
	}
	close(sockfd);
	return 0;
}
