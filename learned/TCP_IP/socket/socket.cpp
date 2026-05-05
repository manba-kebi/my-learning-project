#include "socket.h"
using namespace akai::socket;

Socket::Socket(): m_ip(""),m_port(0),m_sockfd(0)
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);  // 初始化

    // 1. 创建 socket
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    //向操作系统申请一个 套接字（socket），它是一个抽象的通信端点，后续所有收发数据都围绕它进行。
    if (m_sockfd == INVALID_SOCKET) {
        printf("create socket error, code = %d", WSAGetLastError());
        throw std::runtime_error("Failed to create socket");
    }
    printf("create server socket success!\n");
}

Socket::~Socket()
{
    close();
}

bool Socket::bind(const string& ip, int port) {
    struct sockaddr_in sockaddr;
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    if (ip.empty()) {
        sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else {
        inet_pton(AF_INET, ip.c_str(), &sockaddr.sin_addr);   // 修正点1
    }
    sockaddr.sin_port = htons(port);

    if (::bind(m_sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR) {
        printf("bind error, code = %d", WSAGetLastError());
        return false;
    }
    m_ip = ip;
    m_port = port;
    printf("bind success: ip = %s, port = %d\n", ip.c_str(), port);
    return true;
}

bool Socket::listen(int backlog)
{
    // 3. 监听
    if (::listen(m_sockfd, backlog) == SOCKET_ERROR) {
        printf("listen error, code = %d", WSAGetLastError());
        return false;
    }
    printf("server listening ...\n");
    return true;
}

bool Socket::connect(const string& ip, int port) {
    struct sockaddr_in sockaddr;
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &sockaddr.sin_addr);
    sockaddr.sin_port = htons(port);
    if (::connect(m_sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR) {
        printf("socket connect error, code = %d", WSAGetLastError());
        return false;
    }
    m_ip = ip;
    m_port = port;
    return true;
}

SOCKET Socket::accept() {
    SOCKET connfd = ::accept(m_sockfd, nullptr, nullptr);
    if (connfd == INVALID_SOCKET) {
        printf("accept error, code = %d", WSAGetLastError());
        return INVALID_SOCKET;
    }
    printf("socket accept, conn = %d", connfd);
    return connfd;
}

int send(const char* buf, int len) {
    // 6. 发送数据回显
    return ::send(m_sockfd, buf, len, 0);
}

int Socket::recv(char* buf, int len) {
    return ::recv(m_sockfd, buf, len, 0);
}

void Socket::close() {
    if (m_sockfd != INVALID_SOCKET) {
        ::closesocket(m_sockfd);
        WSACleanup();
        m_sockfd = 0;
    }
}