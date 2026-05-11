#pragma once

#include <iostream>
#include <string>

namespace minikv::net {

	class SocketRuntime {
	public:
		SocketRuntime();
		~SocketRuntime();

		SocketRuntime(const SocketRuntime&) = delete;
		SocketRuntime& operator=(const SocketRuntime&) = delete;

		bool ok() const nonexcept;

	private:
		bool initialized_{ false };

	};
	std::string last_socket_error_message();
}