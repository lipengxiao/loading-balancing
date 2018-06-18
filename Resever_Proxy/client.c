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
	char buff[BUFF] = "";
	int res;
	while(1)
	{
		gets(buff);
		fflush(stdin);
		if(strncmp(buff,"end",3) == 0)
		{
			break;
		}
		res = send(sockfd,buff,strlen(buff),0);
		if(res == -1)
		{
			printf("server is err\r\n");
			break;
		}
		memset(buff,0,sizeof(buff));
	}
	close(sockfd);
	return 0;
}
