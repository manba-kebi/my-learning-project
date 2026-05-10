#include "socket_runtime.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#else
#include <cerrno>
#include <cstring>
#endif // !WIN32_LEAN_AND_MEAN


namespace minikv::net {
	SockerRuntime::SocketRuntime() {
#ifdef _WIN32
		WSADATA data{};
		initialized_ = (WSAStartup(MAKEWORD(2, 2), &data) == 0);	//如果初始化成功，WSAStartup 会返回 0
#else
		initialized_ = true;
#endif
	}

	SocketRuntime::~SocketRuntime() {
#ifdef _WIN32
		if (initialized_) {
			WSACleanup();
		}
#endif
		
	}

	bool SocketRuntime::ok()const nonexcept {
		return initialized_;
	}

	std::string last_socket_error_message() {
#ifdef _WIN32
		return "WSA error code: " + std::to_string(WSAGetLastError());
#else
		return std::strerror(errno);
#endif
	}
}