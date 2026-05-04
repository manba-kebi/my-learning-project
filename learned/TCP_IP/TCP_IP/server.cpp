#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <cstring>
#include <string>

using std::string;

int main() {
	//1.创建socket
	int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		//接受三个参数	协议处(TCP/IP固定为AF_INET)、
	if (sockfd < 0) {
		printf("create socket error:error = %d,errmsg=%s", errno, strerror(errno));
		return 1;
	}
	else {
		printf("create socket success!");
	}

	//2.绑定socket
	string ip = "127.0.0.1";
	int port = 8080;

	struct sockaddr_in sockaddr;			//通过这个结构体进行设置好IP端口，然后再通过by函数进行访问
	std::memset(&sockaddr, 0, sizeof(sockaddr));		//结构体初始化，全部初始化成0
	sockaddr.sin_family = AF_INET;			//设置family协议处
	sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());	//IP地址，把主机地址转换成网络地址（网络地址编码都采用大端的，这里要做一个统一）
	sockaddr.sin_port = htons(port);		//端口也要做一个转换
	if (::bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
		printf("socket bind error: errno = %d errmsg = %s", errno, strerror(errno));
		return 1;
	}
	else {
		printf("socket bind success: ip = %s port = %d", ip.c_str(), port);
	}

	//3. 监听 socket
	if (::listen(sockfd, 1024) < 0) {
		printf("socket listen error: errno = %d errmsg = %s", errno, strerror(errno));
		return 1;
	}
	else {
		printf("socket listening ...");
	}

	while (true) {
		//4.接收客户端连接
		int connfd = ::accept(sockfd, nullptr, nullptr);
		if (connfd < 0) {
			printf("socket accept error: errno = %d errmsg = %s", errno, strerror(errno));
			return 1;
		}

		char buf[1024] = { 0 };

		//5.接收客户端的数据
		//size_t len = ::recv(connfd,)
	}
}