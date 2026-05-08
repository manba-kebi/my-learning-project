#pragma once

#include "../socket/socket.h"

namespace akai {
	namespace socket {
		class ServerSocket :public Socket {
		public:
			ServerSocket() = delete;		//默认构造函数不需要，所以删除掉
			ServerSocket(const string& ip, int port);
			~ServerSocket() = default;
		};
	}
}