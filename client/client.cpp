#include <iostream>
//#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
	//����Э��汾
	WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0) {
		printf("����汾ʧ�ܣ�\n");
		return 0;
	}
	printf("����汾�ɹ���\n");

	//����socket
	SOCKET severSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (severSocket == INVALID_SOCKET) {
		printf("����socketʧ�ܣ�\n");
		return -1;
	}
	printf("����socket�ɹ���\n");

	//��ȡ������Э���ַ��
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", (void*)&addr.sin_addr.S_un.S_addr);
	addr.sin_port = htons(10086);

	//���ӷ�����
	int r = connect(severSocket, (sockaddr*)&addr, sizeof addr);
	if (r == -1) {
		printf("�����������ң�\n");
		return 0;
	}
	printf("���ӷ������ɹ���\n");

	//���ӽ���
	char buff[256];
	while (1) {
		memset(buff, 0, 256);
		printf("���뷢�͵����ݣ�");
		scanf("%s", buff);
		r = send(severSocket, buff, strlen(buff), NULL);
		if (r > 0) {
			printf("����%d�ֽڵ��������ɹ���\n", r);
		}
	}
	while (1);
	return 0;
}