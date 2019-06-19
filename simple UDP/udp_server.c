#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BUFFER_LENGTH 1024

int main(int argc, char *argv[]) {
	
	//初始化变量
	struct sockaddr_in server_addr, client_addr;//初始化地址结构
	int sockfd;//初始化套接字返回值变脸
	socklen_t client_addr_len = sizeof(struct sockaddr);//初始化地址长度变量
	char buf[BUFFER_LENGTH]; //缓冲区长度
	int n;

//	如果给定参数不符合规范 报错
//	if (argc != 3){
//		fputs("usage: ./udpserver serverIP serverPort\n", stderr);
//		exit(1);
//	}
	
	// 创建失败报错
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); //指出 网络协议簇 协议类型 特地协议类型
	if (sockfd == -1){
		perror("created udp socket");//失败则报错
		exit(1);
	}
	
	bzero(&server_addr, sizeof(server_addr));//清零
	server_addr.sin_family = AF_INET;// 设置协议类型
	server_addr.sin_port = htons(2333); //将端口设置为2334
	
	char local_address[] = "127.0.0.1"; // 设置IP地址
	if (inet_aton(local_address, &server_addr.sin_addr) == 0)
	{
		perror("inet_aton");
		exit(1);
	}
	
	//绑定IP地址和端口号到套接字
	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
		perror("call to bind");
		exit(1);
	}
	
	//开始执行服务程序
	while(1)
	{
		memset(buf, 0, BUFFER_LENGTH); //初始化缓冲区
		//接收数据
		printf("----begin to receive----\n");
		//接收数据 n为返回长度
		n = recvfrom(sockfd, buf, BUFFER_LENGTH, 0, (struct sockaddr *)&client_addr, &client_addr_len);
		printf("----detect a message----\n");

		if (n == -1)//接收出错 n返回-1
		{
			perror("\nfail to receive\n");
			exit(1);
		}
	
		else
		{
			buf[n] = '\0';
			printf("The messafe is:\n%s\n", buf);
			if(buf[0] == 'e' && buf[1] == 'x' && buf[2] == 'i' && buf[3] == 't')//判断是否需要关闭
			{
				printf("Now close the server!\n");
				if (close(sockfd) == -1)
				{
					perror("fail to close\n");
					exit(1);
				}
				puts("UDP Server is closed!\n");
				return EXIT_SUCCESS;
			}
			printf("Please enter your reply message:\n");
			gets(buf);//从键盘输入回复信息
			printf("The messafe you have entered is: \n%s\n", buf);
			n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));//发送回复信息
			if (n == -1)//发送失败则报错
			{
				perror("fail to reply");
				exit(1);
			}
		
		}
	}
	if (close(sockfd) == -1)//关闭失败则报错
	{
		perror("fail to close\n");
		exit(1);
	}
	puts("UDP Server is closed!\n");
	return EXIT_SUCCESS;
}



