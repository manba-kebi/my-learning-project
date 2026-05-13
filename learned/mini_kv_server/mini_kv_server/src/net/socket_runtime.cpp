/*
 * Copyright (c) 2026 赵晨凱. All rights reserved.
 *
 * 本仓库代码和文档用于个人学习、作品展示和技术交流。未经作者明确许可，不得将本项目代码、文档或截图整体复制后作为个人原创项目发布、参赛或用于商业用途。
 *
 * 如果你参考了本项目中的设计或代码，请在你的仓库或文章中注明来源。公开仓库可以被阅读和学习，但这不代表放弃作者署名权，也不代表允许去除作者信息后重新发布。
 *
 * 如有疑问或需要授权，请联系：2825190599@qq.com
 */

#include "minikv/net/socket_runtime.h"

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
	SocketRuntime::SocketRuntime() {
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

	bool SocketRuntime::ok()const noexcept {
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