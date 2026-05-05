#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#include <string>
#include "socket.h"
using namespace akai::socket;

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);  // 初始化

    // 1. 创建 socket
    SOCKET sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    //向操作系统申请一个 套接字（socket），它是一个抽象的通信端点，后续所有收发数据都围绕它进行。
    if (sockfd == INVALID_SOCKET) {
        printf("create socket error, code = %d", WSAGetLastError());
        return 1;
    }
    else {
        printf("create server socket success!\n");
    }

    // 2. 绑定 socket
    string ip = "127.0.0.1";
    int port = 8080;

    struct sockaddr_in sockaddr;
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &sockaddr.sin_addr);   // 修正点1
    sockaddr.sin_port = htons(port);

    if (::bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR) {
        printf("bind error, code = %d", WSAGetLastError());
        return 1;
    }
    else {
        printf("bind success: ip = %s, port = %d\n", ip.c_str(), port);
    }

    // 3. 监听
    if (::listen(sockfd, 1024) == SOCKET_ERROR) {
        printf("listen error, code = %d", WSAGetLastError());
        return 1;
    }
    else {
        printf("server listening ...\n");
    }

    while (true) {
        // 4. 接收连接
        SOCKET connfd = ::accept(sockfd, nullptr, nullptr);
        if (connfd == INVALID_SOCKET) {
            printf("accept error, code = %d", WSAGetLastError());
            return 1;
        }

        char buf[1024] = { 0 };
        // 5. 接收数据
        int len = ::recv(connfd, buf, sizeof(buf), 0);
        if (len <= 0) {
            printf("client disconnected or error, len = %d\n", len);
            break;
        }
        printf("recv: connfd = %lld, msg = %s\n", (long long)connfd, buf);

        // 6. 发送数据回显
        ::send(connfd, buf, len, 0);


        // 连接处理完记得关闭 connfd，否则资源泄漏
        ::closesocket(connfd);
    }


    return 0;
}