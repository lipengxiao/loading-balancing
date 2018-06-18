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
int create_sockfd(const char*ip,int port);
void run(int fd);


int main(int argc,char*argv[])
{
	if(argc < 3 || argc > 3)
	{
		printf("err argv!\n");
		exit(0);
	}
	char*ip = argv[1];
	int port = atoi(argv[2]);
	int fd = create_sockfd(ip,port);
	if(fd == -1)
	{
		printf("create socket fd err!\n");
		exit(0);
	}
	run(fd);
	return 0;
}

int create_sockfd(const char*ip,int port)
{
	/*create socket fd*/
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd != -1)
	{
		struct sockaddr_in saddr;
		memset(&saddr,0,sizeof(saddr));
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(port);
		saddr.sin_addr.s_addr = inet_addr(ip);
		
		/*bind sockfd and struct of ip&port&protrol*/
		int res = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
		if(res == -1)
		{
			printf("bind err\n");
			exit(0);
		}

		/*create listen queue*/
		listen(sockfd,5);
	}
	return sockfd;
}
void run(int fd)
{
	char buff[BUFF] = {0};
	int res;
	int c_fd;
	struct sockaddr_in caddr;
	socklen_t len = sizeof(caddr);
	while(1)
	{
		c_fd = accept(fd,(struct sockaddr*)&caddr,&len);
		if(c_fd == -1)
		{
			printf("accept err\n");
			break;
		}
		printf("client %d is connected!\n",c_fd);
		fflush(stdout);
		while(1)
		{
			res = recv(c_fd,buff,BUFF-1,0);
			if(res == 0)
			{
				printf("\r\nclient %d over!\r\n",c_fd);
				close(c_fd);
				break;
			}
			printf("%s\n",buff);
			fflush(stdout);
			memset(buff,0,sizeof(buff));
		}
		printf("\r\n");
	}
	close(fd);
}
