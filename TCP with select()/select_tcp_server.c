#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BUFFER_LENGTH 1024
#define SERV_PORT 3001
#define LISTENQ  10


int main(void)
{
	struct sockaddr_in server_addr, client_addr;//创建地址结构
	int listenfd;//创建监听套接字变量
	int connfd;//创建连接套接字变量
	socklen_t client_addr_len = sizeof(struct sockaddr);//创建地址大小变量
	char buf[BUFFER_LENGTH];//创建缓冲区
	int n;
	int	BACKLOG = 5;//最大连接数
	int maxfd;//最大套接字
	
	
	//创建套接字
	// 协议簇 协议类型 特定协议指定
	listenfd = socket(AF_INET, SOCK_STREAM, 0);	
	if (listenfd == -1)
	{
		perror("created TCP socket");
		exit(1);
	}
	
	//初始化server_addr
	bzero(&server_addr, sizeof(server_addr));//清零
	
	char local_addr[] = "127.0.0.1";//设定IP
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
		perror("call to bind");
		exit(1);
	}
	
	//设置未被接收的TCP连接的最大数量
	listen(listenfd, LISTENQ);
	
		
	fd_set allset;
	fd_set rset;
	
	FD_ZERO(&allset);
	FD_ZERO(&rset);
		
	
	FD_SET(listenfd, &allset);
	
	maxfd = listenfd;
	
	while(1)
	{
		rset = allset;//每个循环开始时设置rset
		printf("---begin select---\n");
		select(maxfd+1, &rset, NULL, NULL, NULL);//等待信息
		printf("---select success---\n");
		
		if(FD_ISSET(listenfd, &rset))//判断是否为连接信息
		{
			printf("---new connection---\n");
			if ((connfd = accept(listenfd, NULL, NULL)) == -1)  {
				perror("Accept error.");//建立新连接 失败报错
				 exit(1);
			 }
			maxfd = connfd > maxfd ? connfd : maxfd;//设置新的maxfd
			FD_SET(connfd, &allset);
		}
			
		printf("---connection finish---\n");
		
		if(FD_ISSET(connfd, &rset))//判断是否为套接字数据
		{
			printf("---message detected---\n");
			//开始接收数据
			memset(buf, 0, BUFFER_LENGTH);//清空缓存区
			n = recv(connfd, buf, BUFFER_LENGTH, 0);//开始接收
			if (n == -1){
				perror("fail to receive");//失败报错
				exit(1);
			}
			printf("---read sucessful---\n");
			if(buf[0] == 'e' && buf[1] == 'x' && buf[2] == 'i' && buf[3] == 't')
			{
				close(connfd);//检测到exit口令关闭服务器
				FD_CLR(connfd, &allset);
				continue;
//				return EXIT_SUCCESS;
			}
			for(int i = 0 ; i < n ; i++)
			{
				if(buf[i] <= 'Z' && buf[i] >= 'A')//转换大小写
				{
					continue;
				}
				buf[i] -= 32;
			}
			buf[n] = '\0';
			n = send(connfd, buf, strlen(buf), 0);//发送数据
			if (n == -1)
			{
				perror("fail to reply");//失败报错
				exit(1);
			}
			printf("---reply sucessful---\n");
		}

		
	}
}