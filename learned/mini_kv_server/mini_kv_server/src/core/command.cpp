/*
 * Copyright (c) 2026 赵晨凱. All rights reserved.
 *
 * 本仓库代码和文档用于个人学习、作品展示和技术交流。未经作者明确许可，不得将本项目代码、文档或截图整体复制后作为个人原创项目发布、参赛或用于商业用途。
 *
 * 如果你参考了本项目中的设计或代码，请在你的仓库或文章中注明来源。公开仓库可以被阅读和学习，但这不代表放弃作者署名权，也不代表允许去除作者信息后重新发布。
 *
 * 如有疑问或需要授权，请联系：2825190599@qq.com
 */

#include "minikv/core/command.h"

namespace minikv::core{
	namespace {	//这些函数只给command.cpp使用，所以放到匿名的namespace中
		//去掉字符串两端空白
		std::string_view trim(std::string_view input) {
			std::size_t begin = 0;
			while (begin < input.size() && std::isspace(static_cast<unsigned char>(input[begin])) != 0) {
				++begin;
			}
			//“空白字符”通常包括：
			//	空格 ' '
			//	水平制表符 \t
			//	换行 \n
			//	回车 \r
			//	垂直制表符 \v
			//	换页符 \f
			//调用 C 标准库函数 std::isspace，它能正确识别所有标准空白字符（受当前 locale 影响）

			std::size_t end = input.size();
			while (end > begin && std::isspace(static_cast<unsigned char>(input[end-1])) != 0) {
				end--;
			}
			return input.substr(begin, end - begin);
		}

		//把命令动词转成大写，让 `ping` 和 `PING` 都能识别
		std::string to_upper_copy(std::string_view input) {
			std::string result(input.begin(), input.end());
			//std::transform(输入起点, 输入终点, 输出起点, 转换函数);
			std::transform(result.begin(), result.end(), result.begin(), [](unsigned char ch) {
				return static_cast<char>(std::toupper(ch));
				});
			return result;
			//为什么 std::toupper() 前要转 unsigned char？
			//因为避免未定义行为
			//std::toupper 的参数是 int，但它明确要求参数必须是 unsigned char 的值或 EOF（-1）。
			//如果 char 是有符号类型（常见于 x86 编译器），且字符的最高位是 1（例如 é 的 UTF - 8 字节 0xC3），它的值会是负数。
			//将被直接提升为负 int 传入 toupper 时，不是 EOF，也不在 unsigned char 的合法范围（0~255），结果未定义。
		}

		//统一构造错误命令
		Command invalid_command(std::string message) {
			Command command;
			command.type = CommandType::Invalid;
			command.error = std::move(message);
			return command;
		}
	}
	
	//解析命令
	Command parse_command(std::string_view line) {
		const std::string_view trimmed = trim(line);
		if (trimmed.empty()) {
			return invalid_command("empty command");
		}
		//find_first_of：在字符串中查找第一个与给定字符集合中任意一个字符匹配的位置。
		// 只要字符串中的字符与参数字符序列中的任意一个字符匹配即可
		//如果未找到匹配，则返回一个特殊的常量 std::string::npos
		const std::size_t first_space = trimmed.find_first_of(" \t");	//查找空格或者制表符
		const std::string verb = to_upper_copy(trimmed.substr(0, first_space));
		const std::string_view rest = first_space == std::string_view::npos
			?std::string_view{}
		: trim(trimmed.substr(first_space + 1));

		if (verb == "HELP") {
			//在用聚合初始化（aggregate initialization）创建一个 Command 对象。
			return Command{ CommandType::Help,{},{},{} };
		}
		if (verb == "PING") {
			return Command{ CommandType::Ping,{},{},{} };
		}
		if (verb == "KEYS") {
			return Command{ CommandType::Keys,{},{},{} };
		}
		if (verb == "SIZE") {
			return Command{ CommandType::Size,{},{},{} };
		}
		if (verb == "EXIT" || verb == "QUIT") {
			return Command{ CommandType::Exit,{},{},{} };
		}

		if (verb == "GET" || verb == "DEL") {
			//输入这两个命令时需要输入一个KEY值
			if (rest.empty()) {
				return invalid_command(verb + " requires a key");
			}

			if (rest.find_first_of(" \t") != std::string_view::npos) {
				return invalid_command(verb + " accepts exactly one key");
			}
			return Command{ verb == "GET" ? CommandType::Get : CommandType::Del,
			std::string(rest),
				{},
				{},
			};
		}
		if (verb == "SET") {
			const std::size_t split = rest.find_first_of(" \t");
			if (split == std::string_view::npos) {
				return invalid_command("SET requirees both key and value");
			}
			
			const std::string_view key = rest.substr(0, split);
			const std::string_view value = trim(rest.substr(split + 1));
			//两个字符串都不能为空
			if (key.empty() || value.empty()) {
				return invalid_command("SET ruquires both key and value");
			}

			return Command{ CommandType::Set,
				std::string(key),
				std::string(value),
				{},
			};
		}
		return invalid_command("unknown command: " + verb);
	}

	//返回所有支持命令的说明。
	// 注意每行最后有 `\n`，因为这是发给客户端看的文本协议响应。
	std::string help_message() {
		return
			"Commands:\n"
			"	PING				-> health check\n"
			"	SET <key> <value>	-> create or update a value\n"
			"	GET <key>			-> fetch a value\n"
			"	DEL <key>			-> delete a value\n"
			"	KEYS				-> list all keys\n"
			"	SIZE				-> show key count\n"
			"	EXIT				-> close the client connection\n";
	}

}
