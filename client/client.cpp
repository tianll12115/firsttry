#include <iostream>
//#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	//请求协议版本
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0) {
		printf("请求版本失败！\n");
		return 0;
	}
	printf("请求版本成功！\n");

	//创建socket
	SOCKET severSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (severSocket == INVALID_SOCKET) {
		printf("创建socket失败！\n");
		return -1;
	}
	printf("创建socket成功！\n");

	//获取服务器协议地址族
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", (void*)&addr.sin_addr.S_un.S_addr);
	addr.sin_port = htons(10086);

	//连接服务器
	int r = connect(severSocket, (sockaddr*)&addr, sizeof addr);
	if (r == -1) {
		printf("服务器不理我！\n");
		return 0;
	}
	printf("连接服务器成功！\n");

	//连接建立
	char buff[256];
	while (1) {
		memset(buff, 0, 256);
		printf("输入发送的数据：");
		scanf("%s", buff);
		r = send(severSocket, buff, strlen(buff), NULL);
		if (r > 0) {
			printf("发送%d字节到服务器成功！\n", r);
		}
	}
	while (1);
	return 0;
}