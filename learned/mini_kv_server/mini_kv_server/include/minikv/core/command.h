#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <algorithm>
#include <cctype>

namespace minikv::core {

	//用enum class，是因为避免名字污染：不再有全局的 Help、Set 等，防止和其它枚举或宏冲突。
	//写代码时先写 类型名::，再选值
	//第一个枚举值默认为 0
	//之后每个枚举值依次递增 1（如果没显式赋值的话）
	enum class CommandType {
		Help,
		Ping,
		Set,
		Get,
		Del,
		Keys,
		Size,
		Exit,
		Invalid,

	};
	struct Command {
		CommandType type{CommandType::Invalid};
		std::string key;
		std::string value;
		std::string error;
	};

	Command parse_command(std::string_view line);
	std::string help_message();

}