#pragma once

#include <iostream>
#include <thread>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <vector>
#include <exception>
#include <utility>

namespace minikv::core {
	class ThreadPool {
	public:
		//`Task` 类型用 `std::function<void()>`。
		using Task = std::function<void()>;

		ThreadPool(std::size_t worker_count, std::size_t max_quque_size);
		~ThreadPool();

		ThreadPool(const ThreadPool& ) = delete;
		ThreadPool& operatot = (const ThreadPool & ) = delete;

		bool submit(Task task);
		void shutdown();
		
		std::size_t worker_count() const noexcept;
		std::size_t queue_size() const;

	private:
		void worker_loop();

		mutable std::mutex mutex_;
		std::condition_variable condition_;
		std::queue<Task> tasks_;				//存储待执行的任务
		std::vector<std::thread> workers_;		//存储工作线程对象本身
		std::size_t max_deque_size_{0};
		bool stopping_{ false };
	};
}