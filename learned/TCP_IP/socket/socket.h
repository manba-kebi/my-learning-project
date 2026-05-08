#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")   // 加上这一行
#include <cstring>
#include <string>
#include <stdexcept>
#include <string>
using std::string;

namespace akai
{
	namespace socket
	{
		class Socket
		{
		public:
			Socket();
			Socket(SOCKET sockfd);
			virtual ~Socket();

			bool bind(const string& ip, int port);
			bool listen(int backlog);
			bool connect(const string& ip, int port);
			SOCKET accept();
			int send(const char* buf, int len);
			int recv(char* buf, int len);
			void close();

			//由阻塞IO，设置成非阻塞IO。也就是说不会等待，如果数据没有准备好，就返回error，如果准备好了就返回数据。
			//这种非阻塞的需要不断地去询问数据，不会交出CPU，而会一直占用CPU
			bool set_non_blocking();
			bool set_send_buffer(int size);		//调整发送缓冲区的大小
			bool set_recv_buffer(int size);		//设置接收缓冲区
			bool set_linger(bool active, int seconds);	//这个与close断开连接后是否将剩下未发送的消息继续发送相关
			bool set_keepalive();				//不论是服务端还是客户端，一方开启KeepAlive功能后，就会自动在规定时间内向对方发送心跳包，而另一方在收到心跳包后就会自动回复，以告诉对方我仍然在线
			bool set_reuseaddr();

		protected:
			string m_ip;
			int m_port;
			SOCKET m_sockfd;
		};
	}
}