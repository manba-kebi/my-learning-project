#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")   // 加上这一行
#include <cstring>
#include <string>


using namespace std;

int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//1.创建 socket
	SOCKET sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd == INVALID_SOCKET) {
		printf("create socket error, code = %d", WSAGetLastError());
		return 1;
	}
	else {
		printf("create client socket success!\n");
	}

	//2.连接服务端
	string ip = "127.0.0.1";
	int port = 8080;

	struct sockaddr_in sockaddr;
	std::memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip.c_str(), &sockaddr.sin_addr);
	sockaddr.sin_port = htons(port);
	if (::connect(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR) {
		printf("socket connect error, code = %d", WSAGetLastError());
		return 1;
	}

	//3.向服务端发送数据
	string data = "hello world";
	::send(sockfd, data.c_str(), data.size(), 0);

	//4. 接收服务端的数据
	char buf[1024] = { 0 };
	::recv(sockfd, buf, sizeof(buf), 0);
	printf("recv:&s\n", buf);

	//5.关闭socket
	::closesocket(sockfd);
	WSACleanup();
	return 0;
}