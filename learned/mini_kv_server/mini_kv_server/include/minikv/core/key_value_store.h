#pragma once

#include <iostream>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <map>
#include <optional>
#include <vector>
#include <algorithm>
#include <utility>

namespace minikv::core {
	
	class KeyValueStore {
	public:
		void set(std::string key, std::string value);
		std::optional<std::string> get(const std::string& key) const;
		bool erase(const std::string& key);
		std::vector<std::string> keys() const;
		std::size_t size() const;

	private:
		std::unordered_map<std::string, std::string> data_;
		mutable std::shared_mutex mutex_;
		//std::mutex是普通的互斥锁，它提供独占所有权：同一时间只有一个线程可以锁定它。无论是读还是写，都不能并发。
		
		//std::shared_mutex（C++17引入）是一种共享互斥锁，支持两种锁定模式：
		//独占锁定（写锁）：类似普通互斥，只有一个线程可以获得独占锁，此时其他线程不能获得任何锁（独占或共享）。
		//共享锁定（读锁）：多个线程可以同时获得共享锁，只要没有线程持有独占锁。共享锁允许并发读取访问。

		//mutable 关键字告诉编译器：这个成员变量即使在 const 对象中也能被修改。
		//因为在代码中，很多成员函数都是const的，例如std::vector<std::string> keys() const; 这些函数承诺不修改对象的“逻辑状态”
		//lock 操作会修改 mutex_ 的内部状态（改变锁计数器、所有者等），这在物理上是一次修改。
		//如果 mutex_ 不是 mutable，那么 const 成员函数内的 mutex_.lock_shared() 就会编译失败，因为 const 成员函数只能调用成员变量的 const 方法，而加锁不是 const 操作。
	};
}