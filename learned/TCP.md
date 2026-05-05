# TCP

## 声明

此篇文档是跟着bilibili上的 **天天写乱码**（[高级篇 -- 网络编程基础 - 飞书云文档](https://my.feishu.cn/docx/IBc1dMjZgotxnkxrnO3ctr1ynBa)） 博主所写的，大部分内容也都是从中借用过来的。

 

## TCP/IP四层模型

![image-20260504204959102](C:\Users\28251\AppData\Roaming\Typora\typora-user-images\image-20260504204959102.png)

1. 应用层：主要负责为用户提供网络服务。应用层协议包括`HTTP`（网页、客户端请求服务端接口、GET、POST）、`FTP`（文件传输）、`SMTP`（发邮件）等。
2. 传输层：主要负责在网络中建立端到端的连接，提供可靠的数据传输。传输层协议包括 `TCP` 和 `UDP` 。
3. 网络层：主要负责网络地址的分配和路由选择，例如 `IP` 协议
4. 数据链路层：主要负责传输数据帧，例如以太网、 `ATM` 和 `PPP` 等协议。



## 进程间通信：IPC

1. 管道：pipe
2. 信号：signal
3. 信号量：semaphore
4. 消息队列：message
5. 共享内存：share memory
6. 套接字（重点，允许远程进程之间通信）：socket
   1. `unix socket`：指的是本地两个进程之间也可以用`socket`通信



## 套接字：socket

![image-20260504205043941](C:\Users\28251\AppData\Roaming\Typora\typora-user-images\image-20260504205043941.png)

**服务端启动流程**：先创建一个socket -> 然后将socket绑定到本地的一个IP和端口 -> 绑定好了，监听端口，有没有请求发往这个端口（web server一般监听在80端口，https一般都监听在43端口，SSH默认监听22端口，mysql默认监听3306端口） ->  客户端connect 连接过来，服务端accept函数被触发，返回客户端对应的套接字

**客户端**连服务端之前也会创建一个socket -> 客户端会调用connect连接（传进去IP地址、端口号）

1. 服务端和客户端初始化Socket，得到文件描述符
2. 服务端调用bind，绑定IP和端口
3. 服务端调用listen，进行监听
4. 服务端调用accept，等待客户端连接
5. 客户端调用connect，向服务端发起连接请求。（**TCP三次握手**）
6. 服务端调用accept返回用于传输的Socket的文件描述符（和第一点得到的Socket不同）
7. 客户端使用write写入数据，服务端调用read读取数据（采用双工通信方式）
8. 客户端断开连接时会调用close，服务端也会调用close（**TCP四次挥手**）



```c++
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")   // 加上这一行
#include <cstring>
#include <string>

using std::string;

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
        printf("create socket success!\n");
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
        printf("recv: connfd = %lld, msg = %s\n", (long long)connfd, buf);

        // 6. 发送数据回显
        ::send(connfd, buf, len, 0);

        // 连接处理完记得关闭 connfd，否则资源泄漏
        ::closesocket(connfd);
    }

    ::closesocket(sockfd);
    WSACleanup();
    return 0;
}
```



###  创建通讯端点

```c++
SOCKET sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
```

- **三个参数的含义**：
  - `AF_INET`：地址族，代表 **IPv4**。
  - `SOCK_STREAM`：套接字类型，流式套接字，即 **TCP**。
  - `IPPROTO_TCP`：指定协议为 **TCP**。其实前两个参数已经能推出 TCP，这里显式写出更严谨。
- **返回值**：Windows 下返回 `SOCKET` 句柄（成功）或 `INVALID_SOCKET`（失败）。



### 指定地址类型

```c++
sockaddr.sin_family = AF_INET;
```

- 这是 `sockaddr_in` 结构体的成员，专门存放 **地址族**。
- 值仍然是 `AF_INET`，告诉系统：“这个地址是一个 **IPv4 地址**”。
  虽然创建 socket 时已经指定过，但在“绑定”这一步仍需说明该结构体内存储的是哪种地址，因为还存在 `AF_INET6`（IPv6）等其他类型。



### 将 IP 字符串转为二进制格式，并存入地址结构

```c++
inet_pton(AF_INET, ip.c_str(), &sockaddr.sin_addr);
```

- **`inet_pton` 函数**：把人类可读的 IP（如 `"127.0.0.1"`）转换成网络传输需要的二进制格式，并 **直接写入** 第三个参数指向的 `in_addr` 结构体中。
  - 参数 1：地址族（`AF_INET`）
  - 参数 2：IP 字符串
  - 参数 3：**目标地址的指针**（此处是 `&sockaddr.sin_addr`）
- 它比老旧的 `inet_addr` 更安全，还能处理 IPv6。
- **等价效果**：把 `"127.0.0.1"` 变成 `0x7f000001`（大端序），存放在 `sockaddr.sin_addr` 里。



### 设置端口号，并转换为网络字节序

```c++
sockaddr.sin_port = htons(port);
```

- `port` 是 `int` 型整数，比如 `8080`。
- `htons` = **H**ost **to** **n**etwork **s**hort
  功能：将 **主机字节序**（可能小端）的 16 位短整型，强制转换为 **网络字节序**（大端）。
- 因为 IP 协议规定多字节数值必须用大端序传输，而我们的 x86/ARM 处理器通常是小端序，所以端口号必须转换，否则对方会解析为完全不同的值。
- 转换后赋值给 `sin_port`，这样网络另一端才能正确识别“端口 8080”。



### 将 socket 与地址绑定

```c++
if (::bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR)
```

- **`bind` 函数**：把之前创建的 socket 和我们刚刚填好的地址（IP + 端口）**捆绑**在一起。
  - 参数 1：要绑定的 socket 描述符。
  - 参数 2：指向地址结构体的指针，但这里要求的是通用 `sockaddr*` 类型，所以要把 `sockaddr_in*` **强制转换**。
  - 参数 3：地址结构体的大小（用于内部拷贝校验）。
- **作用**：就像给电话线分配了一个固定的号码，以后客户端就能通过这个 IP + 端口找到我们。
- **返回值**：成功返回 0，失败返回 `SOCKET_ERROR`（Windows 常量，值为 -1）。
  所以这句 `if` 就是判断绑定是否失败。



### **从“等待连接”到“正式通话”**

```c++
SOCKET connfd = ::accept(sockfd, nullptr, nullptr);
```

#### 三个参数详解

- **`sockfd`**：是之前用 `socket()` 创建、再通过 `bind()` + `listen()` 设置好的**监听 socket**。它只负责“接待”，不负责具体的“会话”。
- **第二个参数**（通常是指向 `sockaddr` 的指针）：可以用来获取**客户端的 IP 和端口**。这里写成 `nullptr`，意思是我们暂时不关心是谁连过来的，只接受连接。
- **第三个参数**（通常是地址结构长度的指针）：配合第二个参数使用，这里传 `nullptr` 也表示忽略。

#### 返回值 `connfd` 的意义

- 成功后返回一个**全新的 `SOCKET`**，专门用于和这个连进来的客户端通信。
- 原来那个 `sockfd`（监听 socket）依然保持监听状态，可以继续接收其他客户端的连接（通常下一个循环会再次调用 `accept`）。
- 如果失败，返回 `INVALID_SOCKET`，后面代码的判断 `if (connfd == INVALID_SOCKET)` 就是在做错误处理。







