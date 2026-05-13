#pragma once

#include <iostream>
#include <string>
#include <cstdint>
#include <optional>
#include <string_view>
#include <algorithm>
#include <cstddef>
#include <limits>


#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

namespace minikv::net {
#ifdef _WIN32
	using NativeSocket = SOCKET;	//using 是在 C++ 中定义类型别名的现代写法，等价于typedef SOCKET NativeSocket
	constexpr NativeSocket kInvalidSocket = INVALID_SOCKET;			//constexpr在这里的作用是保证“NativeSocket”这个常量在编译期完全确定，并且可以用在任何要求编译期常量的地方。
#else
	using NativeSocket = int;
	constexpr NativeSocket kInvalidSocket = -1;
#endif

	class Socket {
	public:
		Socket() = default;
		explicit Socket(NativeSocket handle);
		~Socket();

		Socket(const Socket&) = delete;
		Socket& operator = (const Socket&) = delete;

		Socket(Socket&& s) noexcept;
		Socket& operator = (Socket&& s) noexcept;

		static std::optional<Socket> create_tcp();

		bool is_valid()const noexcept;
		NativeSocket native_handle() const noexcept;

		bool set_reuse_address();
		bool bind_and_listen(std::uint16_t port, int backlog);
		std::optional<Socket> accept() const;

		int receive(char* buffer, int length)const;
		bool send_all(std::string_view data)const;

		void shutdown_both();
		void close();
	private:
		NativeSocket handle_ = kInvalidSocket;
	};
}