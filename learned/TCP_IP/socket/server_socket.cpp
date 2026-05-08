#include "server_socket.h"

using namespace akai::socket;

ServerSocket::ServerSocket(const string& ip, int port):Socket()
{
	//set_non_blocking();	现在的例子都是在阻塞环境下的，所以关闭
	set_recv_buffer(10 * 1024);
	set_send_buffer(10 * 1024);
	set_linger(true, 0);
	set_keepalive();
	set_reuseaddr();
	bind(ip, port);
	listen(1024);
}