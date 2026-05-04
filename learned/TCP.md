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