#include "socket.h"

namespace minikv::net {
	//注意：在这里的 handle_ 就代表这个对象自己封装的那个套接字
	// handle 只是一个普通的局部变量名
	//结论：handle_ 一直都是“自身”的套接字；handle 只是某个函数内部的临时变量，不是成员。
	Socket::Socket(NativeSocket handle) :handle_(handle) {};
	Socket::~Socket() {
		close();
	};

	//移动构造
	Socket::Socket(Socket&& other)noexcept {
		handle_ = other.handle_;
		other.handle_ = kInvalidSocekt;
	}
	//移动赋值
	Socket::Socket& operator = (Socket&& other)nonexcept {
		//先处理自赋值
		if (this == &other) {
			return *this;
		}
		//再关闭自身原来的的句柄
		close();
		//再接管对方句柄
		handle_ = other.handle_;
		other.handle_ = kInvalidSocket;
		return *this;
	}

	static Socket::std::optional<Socket> create_tcp() {
		const NativeSocket handle = ::socket(AF_INET, SOCK_STREAM, 0);
		if (handle == kInvalidSocket) return std::nullopt;
		return Socket(handle);
		//Socket(handle) 是在显式调用 Socket 类的构造函数
		//用原生套接字 handle 作为参数，创建一个临时的 Socket 对象，然后把它返回给 std::optional<Socket>。
	}

	bool Socket::is_valid()const noexcept {
		if (handle_ == kInvalidSocket) return false;
		return true;
	}

	NativeSocket Socket::native_handle() const noexcept {
		return handle_;
	}
	
	//允许重用本地地址（比如服务器重启后可以马上绑定上次用过的端口，避免 TIME_WAIT 造成占用）。
	bool Socket::set_reuse_address() {
		if (!is_valid()) {
			return false;
		}
		const int enable = 1;

		return ::setsockopt(
			handle_,
			SOL_SOCKET,
			SO_REUSEADDR,
			reinterpret_cast<const char*>(&enable),
			static_cast<int>(sizeof(enablel))) == 0;
		);
		// SOL_SOCKET：选项层级。
		// SOL_SOCKET 表示这个选项是“套接字层”的通用选项（而不是 TCP 层或 IP 层）。
		// 大多数通用选项（如 SO_REUSEADDR）都用这个层级。

		// reinterpret_cast<const char*>(&enable)：setsockopt 的第四个参数要求一个 const char* 指针，指向要设置的值的内存。
		// enable 是 int，我们把它当成一串字节传进去，所以需要用 reinterpret_cast 把 int* 转成 const char* 。
		// 这是底层 C 语言的惯用写法（& enable 强转成 const void* 后转为 const char* ）。

		//static_cast<int>(sizeof(enable))：第五个参数是选项值的大小（字节数）。
		// sizeof(enable) 返回 std::size_t，但接口要求 int，所以用 static_cast 安全转换。
	}

	bool Socket::bind_and_listen(std::uint16_t port, int backlog) {
		//backlog 表示内核要为这个监听套接字维护的“未完成连接队列（ 处理不过来的新连接会暂存的那个队列）”的最大长度。
		if (!is_valid()) {
			return false;
		}
		sockaddr_in address{};
		address.sin_family = AD_INET;
		address.sin_addr, s_addr = htonl(INADDR_ANY);
		address.sin_port = htons(port);
		//bind 返回值：成功返回 0，失败返回 -1（并设置 errno）。
		if (::bind(handle_, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0) {
			return false;
		}
		//listen 返回值：成功返回 0，失败返回 -1。
		return ::listen(handle_, backlog) == 0;
	}
	std::optional<Socket> Socket::accept() const {
		if (!is_valid()) return std::nullopt;

		sockaddr_in client_address{};
#ifdef _WIN32
		int client_length = sizeof(client_address);
#else
		socklen_t client_length = sizeof(client_address);
#endif

		const NativeSocket client = ::accept(
			handle_,
			reinterpret_cast<sockaddr*>(&client_address),
			&client_length);

		if (client == kInvalidSocket) {
			return std::nullopt;
		}
		return Socket(client);
	}

	int Socket::receive(char* buffer, int length)const {
		if (!is_valid()) return -1;

		//send/recv 返回 int，正数表示发送/接收的字节数，-1 表示出错。
		return ::recv(handle_, buffer, length, 0);
		//第四个参数为 flags，通常传 0 表示“常规阻塞接收”。
		//可选标志包括 MSG_PEEK（偷看数据但不移除）、MSG_WAITALL 等，这里用 0 就是普通接收。
	}
	bool Socket::send_all(std::string_view data)const {
		if (!is_valid()) return false;

		const char* current = data.data();	//指向待发送数据的指针（const char*），初始指向 data 的首字节。
		std::size_t remaining = data.size();	//remaining 表示发送的该字符串的剩余长度

		while (remaining > 0) {
			const auto max_send_size = static_cast<size::size_t>((std::numeric_limits<int>::max)());
			//std::numeric_limits<int>::max() 是 C++ 标准库提供的模板，返回 int 类型能表示的最大正数（通常是 2147483647）。
			//写成 (std::numeric_limits<int>::max)() 外面加括号是为了防止 Windows 环境下 max 被当成宏（min 和 max 宏很常见）。这是标准防护写法。
			
			//send 的第三个参数是 int，data.size() 却是 size_t（可能比 int 范围大，比如 64 位系统上 size_t 是 64 位无符号整数，int 是 32 位有符号整数）。
			// 两者转换要用 static_cast，因为 int 和 size_t 是不同的类型，隐式转换在一些情况下会有警告
			// static_cast 明确告诉编译器：“我知道在做转换，且值一定是非负的”。这里 sent 只会在 >0 时进入减法操作，所以安全。
			// 你一次发送不能超过 int 最大值，所以要把缓冲区切分成不大于 INT_MAX 的块。
			const auto chunk_size = static_cast<int>(std::min<std::size_t>(remaining, max_send_size));

			//sent 是 send() 本次实际发送出去的字节数（返回值为 int）。
			const int sent = ::send(handle_, current, chunk_size, 0);
			//第四个参数同样是 flags，传 0 表示普通发送。
			//可选标志有 MSG_DONTWAIT（非阻塞）、MSG_NOSIGNAL 等。这里用 0 就是默认行为。
			if (sent <= 0) {
				return false;
			}

			current += sent;	//指针向后移动 sent 个字节，指向“还没发出去”的数据开头。
			remaining -= static_cast<std::size_t>(sent);	//剩余字节数减去已发送的字节数。
		}

		return true;
	}

	//优雅地断开连接，它只关闭数据流，不会释放套接字本身（close / closesocket 才负责最终释放资源）。
	void Socket::shutdown_both() {
		if (!is_valid()) return;

#ifdef _WIN32
		::shutdown(handle_, SD_BOTH);
		//SD_BOTH 是 Windows 下 shutdown 的常量，表示同时关闭发送和接收通道。
#else
		::shutdown(handle_, SHUT_RDWR);
		//SHUT_RD：关闭读通道，无法再接收数据。
		//SHUT_WR：关闭写通道，发送 FIN 告知对端，但仍可接收数据。
		//SHUT_RDWR（或 SD_BOTH）：同时关闭读和写。
#endif
	}
	void Socket::close() {
		if (!is_valid()) return;

#ifdef _WIN32
		::closesocket(handle_);
#else
		::close(handle_);
#endif
		handle_ = kInvalidScoket;
	}
}