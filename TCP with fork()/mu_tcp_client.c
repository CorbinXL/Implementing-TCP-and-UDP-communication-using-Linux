#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BUFFER_LENGTH 1024
#define SERV_PORT 3001

int main(int argc, char *argv[]) 
{
	//初始化各项数据
	struct sockaddr_in server_addr;
	int connfd;
	char buf[BUFFER_LENGTH];//缓存
	int n;
	
//	如果给出的参数不符合标准 报错
//	if (argc != 3){
//		fputs("usage: ./udpserver serverIP serverPort\n", stderr);
//		exit(1);
//	}
	
	//创建套接字
	connfd = socket(AF_INET, SOCK_STREAM, 0);//协议簇 协议类型 特别协议指定
	
	//初始化地址
	char local_addr[] = "127.0.0.1";
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;//设定协议簇
	server_addr.sin_port = htons(SERV_PORT);//设定端口
	if (inet_aton(local_addr, &server_addr.sin_addr) == 0)//设定IP
	{
		perror("inet_aton");
		exit(1);
	}
	
	//开始连接
	printf("---start connecting server---\n");
	if (connect(connfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("connect");
		exit(1);
	}
	printf("---server connected---\n");
	
	while(1)
	{
		//清空缓冲区
		memset(buf, 0, BUFFER_LENGTH);
		
		//发送信息
		printf("Please enter your message: ");
		gets(buf);
		n = send(connfd, buf, strlen(buf), 0);
		if (n == -1)
		{
			perror("fail to reply");
			exit(1);
		}
		printf("---sent successfully---\n");
		
		//清空缓冲区
		memset(buf, 0, BUFFER_LENGTH);
		
		//接收信息
		printf("---start searching for data---\n");
		n = recv(connfd, buf, BUFFER_LENGTH, 0);
		printf("---message detected---\n");
		if (n == -1){
			perror("fail to receive");
			exit(1);
		}
		else{
			buf[n] = '\0';
			printf("Server message: %s\n", buf);
		}

	}
	
	if (close(connfd) == -1){
		perror("fail to close");
		exit(1);
	}
	puts("TCP Server is closed!\n");
	return EXIT_SUCCESS;
}




