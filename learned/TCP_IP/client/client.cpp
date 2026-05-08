#include "../socket/client_socket.h"

using namespace akai::socket;
using namespace std;

int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	////1.创建 socket
	//Socket client;

	////2.连接服务端
	//client.connect("127.0.0.1", 8080);

	ClientSocket client("127.0.0.1", 8080);

	//3.向服务端发送数据
	string data = "hello world";
	client.send(data.c_str(), data.size());

	//4. 接收服务端的数据
	char buf[1024] = { 0 };
	client.recv(buf, sizeof(buf));
	printf("recv:%s\n", buf);

	//5.关闭socket
	//client.close();		//析构函数会自动关闭
	WSACleanup();
	return 0;
}