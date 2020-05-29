#include <iostream>
//#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

SOCKET clientSocket[1024];
int count = 0;

DWORD communication(LPVOID n) {
	//连接建立
	char buff[256];
    int r;
    int i = (int)n;
	while (1) {
		memset(buff, 0, 256);
		r = recv(clientSocket[i - 1], buff, 255, NULL);
		if (r > 0) {
			printf("%d:%s\n", i, buff);
            for (int j = 0; j < count; ++j) {
                send(clientSocket[j], buff, strlen(buff), NULL);
            }
		}
	}
}

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

    //创建协议地址族:协议、主机、端口
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", (void*)&addr.sin_addr.S_un.S_addr);
    addr.sin_port = htons(10086);

    //绑定
    int r = bind(severSocket, (sockaddr*)&addr, sizeof addr);
    if (r == -1) {
        printf("绑定失败！\n");
        return -1;
    }
    printf("绑定成功！\n");

    //监听
    r = listen(severSocket, 10);
	if (r == -1) {
		printf("监听失败！\n");
		return -1;
	}
	printf("监听成功！\n");

    //等待客户端连接
    SOCKADDR_IN cAddr;
    int len = sizeof cAddr;
    
    int i = 0;
    while (i < 1024) {
		clientSocket[i++] = accept(severSocket, (sockaddr*)&cAddr, &len);
        count++;
		if (clientSocket[i - 1] == SOCKET_ERROR) {
			printf("错误的客户端！\n");
			closesocket(severSocket);
			WSACleanup();
			return 0;
		}
		printf("有客户端连接进来了\n");

        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)communication, (LPVOID)i, NULL, NULL);
    }
    
	while (1);
	return 0;
}