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


int main(void)
{
	
	struct sockaddr_in server_addr, client_addr;//创建地址结构
	int listenfd, connfd;//创建监听套接字和连接套接字变量
	socklen_t client_addr_len = sizeof(struct sockaddr);//创建地址结构长度
	char buf[BUFFER_LENGTH];//创建缓冲区
	int n;
	pid_t pid;//创建fork（）区分值
	int	BACKLOG = 5;
	
	
	//创建套接字
	// 协议簇 协议类型 特定协议指定
	listenfd = socket(AF_INET, SOCK_STREAM, 0);	//创建监听套接字
	if (listenfd == -1)
	{
		perror("created TCP socket");//失败报错
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
		perror("inet_aton");//失败报错
		exit(1);
	}

	
	//绑定地址和套接字
	if (bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("call to bind");//失败报错
		exit(1);
	}
	
	//设置未被接收的TCP连接的最大数量
	listen(listenfd, LISTENQ);
	
	
	while(1) 
	{
		if ((connfd = accept(listenfd, NULL, NULL)) == -1)  {
			perror("Accept error.");//等待连接到来 失败报错
			 exit(1);
		 }
		if ((pid = fork()) >0) //如果是父进程 直接开始下一个循环
		{
			close(connfd);
			continue;
		} 
		else if (pid == 0)  //如果海思子进程
		{
			while(1)
			{
				close(listenfd);//关闭监听端口
				//初始化缓冲区
				printf("---start searching data---\n");
				memset(buf, 0, BUFFER_LENGTH);//清空缓存区
				printf("---message detected---\n");
				
				//开始接收数据
				n = recv(connfd, buf, BUFFER_LENGTH, 0);
				
				if (n == -1){
					perror("fail to receive");//失败报错
					exit(1);
				}
				else
				{
					if(buf[0] == 'e' && buf[1] == 'x' && buf[2] == 'i' && buf[3] == 't')
					{
						if (close(listenfd) == -1){
							perror("fail to close");//判断是否为关闭命令
							exit(1);
						}
						puts("TCP Server is closed!\n");
						close(connfd);//关闭连接
						exit(0);
				}
					//打印缓冲区
					printf("Client message: ");
					buf[n] = '\0'; 
					printf("%s\n", buf);
					
					//发送数据
					memset(buf, 0, BUFFER_LENGTH);
					printf("Please enter your message: ");
					gets(buf);//从键盘读取
					n = send(connfd, buf, strlen(buf), 0);
					if (n == -1)
					{
						perror("fail to reply");//发送失败报错
						exit(1);
					}
				}
			}
		}
		else 
		{
			perror("Create child process failed.");
			exit(1);
		}
	}
}
