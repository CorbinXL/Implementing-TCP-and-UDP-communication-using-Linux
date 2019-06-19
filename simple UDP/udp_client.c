#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BUFFER_LENGTH 1024

int main(int argc, char *argv[]) 
{
	//创建参数
	struct sockaddr_in server_addr;//创建地址结构
	int sockfd;//创建套接字变量
	socklen_t server_addr_len = sizeof(struct sockaddr);//创建地址长度变量
	char buf[BUFFER_LENGTH];//创建缓冲区
	int n;
	
//	判断外界参数是否符合规范
//	if (argc != 3){
//		fputs("usage: ./udpserver serverIP serverPort\n", stderr);
//		exit(1);
//	}
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); //指出 网络协议簇 协议类型 特地协议类型
	
	if (sockfd == -1){
		perror("created udp socket...\n");
		exit(1);
	}
	
	bzero(&server_addr, sizeof(server_addr));//将地址结构先归零
	server_addr.sin_family = AF_INET;//设置协议
	server_addr.sin_port = htons(2333);//设置端口
	
	char local_address[] = "127.0.0.1"; // 设置IP地址
	if (inet_aton(local_address, &server_addr.sin_addr) == 0)//录入IP地址
	{
		perror("inet_aton");//录入失败报错
		exit(1);
	}
	
	while(1)
	{
		memset(buf, 0, BUFFER_LENGTH);//清空缓存
		gets(buf);//读取输入数据
		printf("The messafe you have entered is: \n%s\n", buf);
		//发送数据
		n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
		if (n == -1)//发送失败报错
		{
			perror("fail to send");
			exit(1);
		}
		
		memset(buf, 0, BUFFER_LENGTH); //初始化缓冲区
		//接收数据
		printf("----begin to receive----\n");
		n = recvfrom(sockfd, buf, BUFFER_LENGTH, 0, (struct sockaddr *)&server_addr, &server_addr_len);
		printf("----detect a message----\n");
		if (n == -1)//接收失败报错
		{
			perror("\nfail to receive");
			exit(1);
		}
		else
		{
			buf[n] = '\0';//将数据尾置0方便输出
			printf("The messafe is:\n%s\n", buf);
		}
		
	}
	if (close(sockfd) == -1)
	{
		perror("fail to close\n");
		exit(1);
	}
	puts("UDP Server is closed!\n");
	return EXIT_SUCCESS;
}

		
		
		
		

