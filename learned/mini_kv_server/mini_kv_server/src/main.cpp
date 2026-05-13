/*
 * Copyright (c) 2026 赵晨凱. All rights reserved.
 * 
 * 本仓库代码和文档用于个人学习、作品展示和技术交流。未经作者明确许可，不得将本项目代码、文档或截图整体复制后作为个人原创项目发布、参赛或用于商业用途。
 *
 * 如果你参考了本项目中的设计或代码，请在你的仓库或文章中注明来源。公开仓库可以被阅读和学习，但这不代表放弃作者署名权，也不代表允许去除作者信息后重新发布。
 * 
 * 如有疑问或需要授权，请联系：2825190599@qq.com
 */

//提供命令行入口，初始化 socket runtime，解析参数，启动服务器。

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>

#include "minikv/net/socket_runtime.h"
#include "minikv/server/server.h"

namespace {
	//打印用法
	void print_usage(const char* program){
		std::cout << "Usage: " << program << " [--port <port>] [--workers <count>] [--queue <count>] [--backlog <count>]\n"
			<< "Example: " << program << " --port 9000 --workers 4 --queue 64 --backlog 64\n";
	}

	// 解析端口，范围必须在 uint16_t 以内
	bool parse_u16(const std::string& text, std::uint16_t& value) {
		//std::uint16_t 是 C++11 引入的固定宽度整数类型，定义在 <cstdint> 中，是无符号 16 位整数，范围 0~65535
		char* end = nullptr;
		const unsigned long parsed = std::strtoul(text.c_str(), &end, 10);
		//unsigned long std::strtoul(const char* str, char** endptr, int base);
		//把 C 风格字符串 str 转换成 unsigned long 整数，按给定的进制 base（这里是 10 进制）。
		//设置 endptr 指向第一个无法转换的字符位置。
		//如果完全没有转换任何数字，endptr 会被设为 str 的起始地址。

		//其中char** endptr是个二级指针
		//因为在 C 语言中，如果要让函数修改一个指针本身（而非它指向的内容），就必须传递指针的地址，也就是二级指针。
		//这里把 &end 传给 char** endptr ，*endptr = 停止位置的地址;

		const auto max_port = static_cast<unsigned long>((std::numeric_limits<std::uint16_t>::max)());
		if (end == text.c_str() || *end != '\0' || parsed > max_port) {
			return false;
		}
		value = static_cast<std::uint16_t>(parsed);
		return true;
	}

	bool parse_size(const std::string& text,std::size_t& value) {
		//解析 worker 和 queue
		char* end = nullptr;
		const unsigned long long parsed = std::strtoull(text.c_str(), &end, 10);
		//这里的策略是：解析函数只做格式校验（纯数字、无多余字符），不越权做业务范围校验
		if (end == text.c_str() || *end != '\0') {
			return false;
		}
		value = static_cast<std::size_t>(parsed);
		return true;
	}
}

int main(int argc,char* argv[]) {
	minikv::net::SocketRuntime runtime;
	if (!runtime.ok()) {
		std::cerr << "Failed to initialize socket runtime\n";
		return 1;
	}

	minikv::server::Serverconfig config;

	for (int i = 1; i < argc; ++i) {
		const std::string_view arg = argv[i];
		if (arg == "--help" || arg == "-h") {
			print_usage(argv[0]);
			return 0;
		}

		if (i + 1 >= argc) {
			//每当读到一个像 --port、--workers 这样的选项，它一定需要紧跟一个值（比如 --port 9000）。
			//而 i 是当前选项的位置，而 i+1 是期待值的位置
			//如果 i+1>=argc 就说明argv[i+1]越界了，值不存在了

			std::cerr << "Missing value for argument: " << arg << '\n';
			//std::cerr 是 标准错误输出流，专门用来输出错误信息。

			print_usage(argv[0]);
			return 1;
		}

		const std::string value = argv[++i];

		if (arg == "--port") {
			if (!parse_u16(value, config.port)) {
				std::cerr << "Invalid port: " << value << '\n';
				return 1;
			}
		}
		else if (arg == "--workers") {
			if (!parse_size(value, config.worker_count)) {
				std::cerr << "Invalid worker count: " << value << '\n';
				return 1;
			}
		}
		else if (arg == "--queue") {
			if (!parse_size(value, config.max_queue_size)) {
				std::cerr << "Invalid queue size: " << value << '\n';
				return 1;
			}
		}
		else if (arg == "--backlog") {
			std::size_t backlog = 0;
			const auto max_backlog = static_cast<std::size_t>((std::numeric_limits<int>::max)());
			if (!parse_size(value, backlog) || backlog > max_backlog) {	//类似于上面那个parse_u16函数
				std::cerr << "Invalid backlog: " << value << '\n';
				return 1;
			}
			config.backlog = static_cast<int>(backlog);
		}
		else {
			std::cerr << "Unknown argument: " << arg << '\n';
			print_usage(argv[0]);
			return 1;
		}
	}
	minikv::server::Server server(config);
	if (!server.start()) {
		return 1;
	}

	server.run();
	return 0;
}
