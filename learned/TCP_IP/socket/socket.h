#pragma once
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")   // 加上这一行
#include <cstring>
#include <string>
#include <stdexcept>
using std::string;

namespace akai
{
	namespace socket
	{
		class Socket
		{
			public:
				Socket();
				~Socket();

				bool bind(const string& ip, int port);
				bool listen(int backlog);
				bool connect(const string& ip, int port);
				SOCKET accept();
				int send(const char* buf, int len);
				int recv(char* buf, int len);
				void close();

			protected:
				string m_ip;
				int m_port;
				int m_sockfd;
		}
	}
}