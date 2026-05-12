#pragma once

#include <atomic>
#include <csdint>
#include <memory>
#include <string>

#include "minikv/core/command.h"
#include "minikv/core/key_value_store.h"
#include "minikv/core/thread_pool.h"
#include "minikv/net/socket.h"

#include <array>
#include <cstddef>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

//server:监听端口、接受连接、把连接投递给线程池、读取命令、执行 KV 操作、返回响应。

namespace minikv::server {
	struct Serverconfig {
		std::uint16_t port{ 9000 };	//设定端口
		std::size_t worker_count{ 4 };	//可以并行的最大线程
		std::size_t max_queue_size{ 64 };	//任务队列可以存放任务的最大数量
		int backlog{64};		//表示内核要为这个监听套接字维护的“未完成连接队列（ 处理不过来的新连接会暂存的那个队列）”的最大长度。
	};
	class Server {
	public:
		explicit Server(Serverconfig config);
		~Server();

		bool start();
		void run();


	private:
		enum class ReadLineStatus {
			Ok,
			Closed,
			TooLong,
		};

		ReadLineStatus read_line(const net::Socket& socket,std::string& pending,std::string& line)const;
		void handle_client(const std::shared_ptr<net::Socket>& client);
		std::string execut_command(const core::Command& command,bool& should_cloce);

		ServerConfig config_;
		net::Socket listen_socket_;
		core::ThreadPool pool_;
		core::KeyValueStore store_;
		std::atomic<bool> started_{false};
		//原子布尔变量，用于在多线程环境下安全地记录“服务器是否已启动”

	};
}