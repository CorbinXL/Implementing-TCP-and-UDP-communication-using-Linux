#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#define BUFFER_LENGTH 1024
#define SERV_PORT 3001
#define LISTENQ  10

int main(int argc, char *argv[]) 
{
	//初始化数据
	bool have_link = false;//判断是否有连接
	struct sockaddr_in server_addr, client_addr;//创建地址结构
	int listenfd, connfd;//创建套接字返回值变量
	socklen_t client_addr_len = sizeof(struct sockaddr);//创建地址长度描写变量
	char buf[BUFFER_LENGTH];//创建缓冲区
	int n;
	
	//创建套接字
	listenfd = socket(AF_INET, SOCK_STREAM, 0);// 协议簇 协议类型 特定协议指定
	if (listenfd == -1)//创建失败报错
	{
		perror("created TCP socket");
		exit(1);
	}
	
	//初始化server_addr
	bzero(&server_addr, sizeof(server_addr));
	
	char local_addr[] = "127.0.0.1";
	server_addr.sin_family = AF_INET;// 设置协议簇
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//设置地址
	server_addr.sin_port = htons(SERV_PORT);//设置端口
	if (inet_aton(local_addr, &server_addr.sin_addr) == 0)//设定IP
	{
		perror("inet_aton");//转换失败报错
		exit(1);
	}

	
	//绑定地址和套接字
	if (bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("call to bind");//绑定失败报错
		exit(1);
	}
	
	//设置未被接收的TCP连接的最大数量
	listen(listenfd, LISTENQ);
	
	while(1)
	{
		if(!have_link)
		{
			//开始监听连接请求
			printf("---start listening---\n");
			connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_addr_len);
			printf("---link detected---\n");
			if(connfd < 0)
			{
				perror("accept");//监听失败报错
				exit(1);
			}
			else
			{
				have_link = true;//连接判定改为真
			}
		}
		
		//初始化缓冲区
		printf("---start searching data---\n");
		memset(buf, 0, BUFFER_LENGTH);
		printf("---message detected---\n");
		
		//开始接收数据
		n = recv(connfd, buf, BUFFER_LENGTH, 0);
		
		if (n == -1){
			perror("fail to receive");//接收失败报错
			exit(1);
		}
		else
		{
			//判断是否需要关闭服务器
			if(buf[0] == 'e' && buf[1] == 'x' && buf[2] == 'i' && buf[3] == 't')
			{
				if (close(listenfd) == -1){
					perror("fail to close");
					exit(1);
				}
				puts("TCP Server is closed!\n");
				return EXIT_SUCCESS;
			}
			//打印缓冲区
			printf("Client message: ");
			buf[n] = '\0'; 
			printf("%s\n", buf);
			
			//发送数据
			printf("Please enter your mesage: ");
			gets(buf);//从见哦安读取数据
			n = send(connfd, buf, strlen(buf), 0);
			if (n == -1)
			{
				perror("fail to reply");
				exit(1);
			}
		}
	}
	if (close(listenfd) == -1){
		perror("fail to close");
		exit(1);
	}
	puts("TCP Server is closed!\n");
	return EXIT_SUCCESS;
}


