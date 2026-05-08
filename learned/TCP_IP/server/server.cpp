
#include "../socket/server_socket.h"
using namespace akai::socket;

int main() {
    // 初始化 WinSock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //// 1. 创建 socket
    //Socket server;

    //// 2. 绑定 socket
    //string ip = "127.0.0.1";
    //int port = 8080;

    //server.bind("127.0.0.1", port);

    //// 3. 监听
    //server.listen(1024);

    ServerSocket server("127.0.0.1", 8080);

    while (true) {
        // 4. 接收连接
        SOCKET connfd = server.accept();
        Socket client(connfd);

        char buf[1024] = { 0 };
        // 5. 接收数据
        int len = client.recv( buf, sizeof(buf));
        if (len <= 0) {
            printf("client disconnected or error, len = %d\n", len);
            break;
        }
        printf("recv: connfd = %lld, msg = %s\n", (long long)connfd, buf);

        // 6. 发送数据回显
        client.send(buf, len);
        
    }

    //7.关闭socket
    //server.close();
    WSACleanup();
    return 0;
}