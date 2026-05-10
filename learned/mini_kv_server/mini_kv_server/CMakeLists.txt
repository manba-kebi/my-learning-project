cmake_minimum_required(VERSION 3.16)

project(mini_kv_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(minikv					# 创建一个名为 minikv 的静态库（默认类型），由列出的 6 个源文件编译而成。
	src/net/socket_runtime.cpp
	src/net/socket.cpp
	src/core/thread_pool.cpp
	src/core/key_value_store.cpp
	src/core/command.cpp
	src/server/server.cpp
)

target_include_directories(minikv
	PUBLIC
		${PROJECT_SOURCE_DIR}/include
)
if(WIN32)
	target_compile_definitions(minikv PUBLIC WIN32_LEAN_AND_MEAN NOMINMAX) # 排除 Windows 头文件中不常用的 API 以加快编译 以及禁止 windows.h 定义 min/max 宏
endif()

if(MSVC)
	target_compile_options(minikv PRIVATE /W4 /permissive-)		# 在 MSVC 编译器下，添加编译选项 /W4（高警告级别）和 /permissive-（更严格的标准一致性模式）
else()
	target_compile_options(minikv PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WIN32)
	target_link_libraries(minikv PRIVATE ws2_32)				# 在 Windows 平台下，为 minikv 库链接 ws2_32（Winsock 库），提供网络通信支持。
endif()

add_executable(mini_kv_server src/main.cpp)
target_link_libraries(mini_kv_server PRIVATE minikv)
