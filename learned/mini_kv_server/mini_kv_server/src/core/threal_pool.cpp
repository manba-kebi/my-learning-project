#include "minikv/core/thread_pool.h"

namespace minikv::core {

	ThreadPool::ThreadPool(std::size_t worker_count, std::size_t max_quque_size):
		max_queue_size_(max_queue_size) {
		if (worker_count == 0) {
			worker_count = 1;
		}
		if (max_queue_size == 0) {
			max_queue_size = 1;
		}
		//创建 worker 线程，每个线程运行 `worker_loop()`。
		for (std::size_t i = 0; i < worker_count; ++i) {
			//循环是为了创建 worker_count 个并发的工作线程。
			//是多线程世界里的标准模式：多个工人，共享一个任务池

			workers_.emplace_back([this] {worker_loop(); });
			//[this] { worker_loop(); } 是一个 lambda 表达式：
			//[this]：捕获当前对象的 this 指针，让 lambda 内部能调用成员函数 worker_loop。
			//{ worker_loop(); }：函数体，就是执行那个循环。
		}
	}
	ThreadPool::~ThreadPool() {
		shutdown();
	}

	bool ThreadPool::submit(Task task) {
		std::lock_guard<std::mutex> lock(mutex_);
		//如果已经停止，返回 false。
		if (stopping_) return false;
		//如果队列满了，返回 false。
		if (tasks_.size() >= max_queue_size_) return false;

		//把任务 move 进队列。
		tasks_.push(std::move(task));
		//`notify_one()` 唤醒一个 worker。
		condition_.notify_one();
		return true;
	}

	void ThreadPool::shutdown() {
		{
			std::lock_guard<std::mutex> lock(mutex_);
			if (stopping_) {
				return;
			}
			stopping_ = true;
		}
		condition_.notify_all();
		//join 所有线程。
		for (auto& worker : workers_) {
			if (worker.joinable()) {
				worker.join();
			}
		}
	}

	std::size_t ThreadPool::worker_count() const noexcept {
		return workers_.size();
	}
	std::size_t ThreadPool::queue_size() const {
		std::lock_guard<std::mutex> lock(mutex_);
		return tasks_.size();
	}

	void ThreadPool::worker_loop() {
		while (true) {
			Task task;
			{
				std::unique_lock<std::mutex> lock(mutex_);
				//锁保证同一时刻只有一个线程能从 tasks_ 里拿到任务。
				//所以线程们虽然都是跑 worker_loop，但拿到的 task 是不同的，处理效果自然就是并行的。
				condition_.wait(lock, [this] {
					return stopping_ || !task_.empty();
					});
				//工作线程一直睡眠，直到以下任一条件成立才醒来继续执行：
				//stopping_ == true：线程池正在停止。
				//!tasks_.empty()：任务队列里有活可干。
				if (stopping_ && tasks_.empty()) {
					return ;
				}

				task = std::move(task_.front());
				tasks_.pop();
			}
			try {
				task();
			}
			catch (const std::exception& ex) {		
				//如果 task() 抛出异常，首先尝试匹配 std::exception 及其子类（如 std::runtime_error 等）。
				//匹配成功：用 ex.what() 打印异常消息，但程序不崩溃。
				std::cerr << "[worker] task threw exception: " << ex.what() << '\n';
			}
			catch (...) {
				std::cerr << "[worker] task threw unknown exception\n";
			}

		}
	}
}