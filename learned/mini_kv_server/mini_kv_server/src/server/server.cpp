/*
 * Copyright (c) 2026 赵晨凱. All rights reserved.
 *
 * 本仓库代码和文档用于个人学习、作品展示和技术交流。未经作者明确许可，不得将本项目代码、文档或截图整体复制后作为个人原创项目发布、参赛或用于商业用途。
 *
 * 如果你参考了本项目中的设计或代码，请在你的仓库或文章中注明来源。公开仓库可以被阅读和学习，但这不代表放弃作者署名权，也不代表允许去除作者信息后重新发布。
 *
 * 如有疑问或需要授权，请联系：2825190599@qq.com
 */

#include "minikv/server/server.h"

#include "minikv/net/socket_runtime.h"
namespace minikv::server {
	namespace {
		std::mutex g_log_mutex;		//全局日志锁，避免多线程日志交错

		void log_line(const std::string& message) {
			std::lock_guard<std::mutex> lock(g_log_mutex);
			std::cout << message << std::endl;
		}

		std::string join_keys(const std::vector<std::string>& keys) {
			//后面的execute_command()模块会用到
			std::ostringstream output;
			//std::ostringstream 内存中的字符串拼接工具，像 cout 一样用 << 写东西，最后用 .str() 取出完整的字符串。
			output << "KEYS";

			for (const auto& key : keys) {
				output << ' ' << key;
			}
			output << '\n';
			return output.str();
		}
	}
	Server::Server(Serverconfig config) :config_(config),
		pool_(config.worker_count, config.max_queue_size) {
	}

	bool Server::start() {
		auto socket = net::Socket::create_tcp();
		if (!socket.has_value()) {
			log_line("[server] failed to create listen socket: " + net::last_socket_error_message());
			return false;
		}

		if (!socket->set_reuse_address()) {
			log_line("[server] warning: failed to enable SO_REUSEADDR");
		}

		if (!socket->bind_and_listen(config_.port, config_.backlog)) {
			log_line("[server] bind/listen failed: " + net::last_socket_error_message());
			return false;
		}

		listen_socket_ = std::move(*socket);
		//这里 socket 的类型是 std::optional<Socket>
		started_ = true;

		log_line(
			"[server] listening on port " + std::to_string(config_.port) +
			",workers=" + std::to_string(pool_.worker_count()) +
			",queue=" + std::to_string(config_.max_queue_size));
		return true;
	}

	void Server::run() {
		if (!started_) {
			log_line("[server] start() must succeed before run()");
			return;
		}

		while (true) {
			auto client = listen_socket_.accept();
			if (!client.has_value()) {
				log_line("[server] accept failed: " + net::last_socket_error_message());
				continue;
			}

			auto client_ptr = std::make_shared<net::Socket>(std::move(*client));
			//client 也是 std::optional<Socket>

			const bool accepted = pool_.submit([this, client_ptr] {
				handle_client(client_ptr);
				});

			if (!accepted) {
				client_ptr->send_all("BUSY server queue is full ,retry later\n");
				client_ptr->shutdown_both();
				client_ptr->close();
			}
		}
	}

	Server::ReadLineStatus Server::read_line(const net::Socket& socket, std::string& pending, std::string& line)const {
		//socket：客户端连接
		//pending：外部传入的持久缓冲区，存储上次未处理完的数据
		//line：输出参数，存解析出的一行文本（不含换行符）
		constexpr std::size_t kMaxLineLength = 4096;	// 行最大长度
		std::array<char, 1024> buffer{};				// 临时缓冲区，每次最多读1024字节

		while (true) {
			const std::size_t newline = pending.find('\n');
			if (newline != std::string::npos) {
				line = pending.substr(0, newline);		// 截取换行符之前的内容作为一行
				pending.erase(0, newline + 1);			// 从 pending 中删除已取出的部分（包括\n）

				if (!line.empty() && line.back() == '\r') {
					//在 Telnet 或许多文本协议中，行结束标记是 \r\n（回车 + 换行）
					line.pop_back();					 // 去掉可能存在的 '\r'（处理 Windows 风格 \r\n）
				}

				return ReadLineStatus::Ok;
			}

			if (pending.size() > kMaxLineLength) {
				return ReadLineStatus::TooLong;
			}

			const int received = socket.receive(buffer.data(), static_cast<int>(buffer.size()));
			//返回值 > 0：实际读取到的字节数。
			//返回值 == 0：对方优雅关闭连接（发送了 FIN）。
			//返回值 < 0（-1）：发生错误。
			if (received <= 0) {		//// 连接断开或错误
				return ReadLineStatus::Closed;
			}

			pending.append(buffer.data(), static_cast<std::size_t>(received));
		}
	}

	void Server::handle_client(const std::shared_ptr<net::Socket>& client) {
		client->send_all("WELCOME mini_kv_server.Type HELP for commands.\n");

		std::string pending;
		std::string line;

		while (true) {
			const ReadLineStatus status = read_line(*client, pending, line);
			if (status == ReadLineStatus::Closed) {
				return;
			}

			if (status == ReadLineStatus::TooLong) {
				client->send_all("ERR command line is too long\n");
				client->shutdown_both();
				return;
			}

			bool should_close = false;
			const core::Command command = core::parse_command(line);
			const std::string response = execute_command(command, should_close);

			if (!client->send_all(response)) {
				//把响应发回客户端。
				return;
			}

			if (should_close) {
				client->shutdown_both();
				return;
			}
		}
	}

	std::string Server::execute_command(const core::Command& command, bool& should_close) {
		switch (command.type) {
		case core::CommandType::Help:
			return core::help_message();

		case core::CommandType::Ping:
			return "PONG\n";

		case core::CommandType::Set:
			store_.set(command.key, command.value);
			return "OK\n";

		case core::CommandType::Get: {
			const auto value = store_.get(command.key);
			if (!value.has_value()) {
				return "NOT_FOUND\n";
			}
			return "VALUE " + *value + '\n';
		}

		case core::CommandType::Del:
			return store_.erase(command.key) ? "DELETED\n" : "NOT_FOUND\n";

		case core::CommandType::Keys:
			return join_keys(store_.keys());

		case core::CommandType::Size:
			return "SIZE " + std::to_string(store_.size()) + '\n';

		case core::CommandType::Exit:
			should_close = true;
			return "BYE\n";

		case core::CommandType::Invalid:
			return "ERR " + command.error + '\n';
	}
	return "ERR unhandled command\n";
	}
}