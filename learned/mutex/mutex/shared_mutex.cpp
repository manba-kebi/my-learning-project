#include <iostream>
#include <shared_mutex>
#include <thread>
#include <mutex>
#include <syncstream>

class ThreadSafeCounter {
public:
	ThreadSafeCounter() = default;

	//多个线程 / 读取器可以同时读取计数器的值。
	unsigned int get() const {
		std::shared_lock lock(mutex_);
		return value_;
	}

	// 只有一个线程/写入器可以增加/写入计数器的值。
	void increment() {
		std::unique_lock lock(mutex_);
		++value_;
	}

	//只有一个线程/写入器可以重置/写入计数器的值。
	void reset() {
		std::unique_lock lock(mutex_);
		value_ = 0;
	}

private:
	mutable std::shared_mutex mutex_;
	unsigned int value_{};
};

int main() {
	ThreadSafeCounter counter;

	auto increment_and_print = [&counter]() {
		for (int i{}; i != 3; ++i) {
			counter.increment();
			std::osyncstream(std::cout)
				<< std::this_thread::get_id() << ' ' << counter.get() << '\n';
		}
		};

	std::thread thread1(increment_and_print);
	std::thread thread2(increment_and_print);

	thread1.join();
	thread2.join();
}