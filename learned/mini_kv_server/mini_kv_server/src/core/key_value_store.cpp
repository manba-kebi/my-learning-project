/*
 * Copyright (c) 2026 赵晨凱. All rights reserved.
 *
 * 本仓库代码和文档用于个人学习、作品展示和技术交流。未经作者明确许可，不得将本项目代码、文档或截图整体复制后作为个人原创项目发布、参赛或用于商业用途。
 *
 * 如果你参考了本项目中的设计或代码，请在你的仓库或文章中注明来源。公开仓库可以被阅读和学习，但这不代表放弃作者署名权，也不代表允许去除作者信息后重新发布。
 *
 * 如有疑问或需要授权，请联系：2825190599@qq.com
 */

#include "minikv/core/key_value_store.h"

namespace minikv::core {
	//写操作，用 `std::unique_lock<std::shared_mutex>`。
	//读操作，用 `std::shared_lock<std::shared_mutex>`。
	void KeyValueStore::set(std::string key, std::string value) {
		std::unique_lock<std::shared_mutex> lock(mutex_);
		data_[std::move(key)] = std::move(value);
	}

	std::optional<std::string> KeyValueStore::get(const std::string& key) const {
		std::shared_lock<std::shared_mutex> lock(mutex_);

		const auto it = data_.find(key);	//确认是否有这个 key ,以及获取他的位置
		if (it == data_.end()) return std::nullopt;
		return it->second;
	}

	bool KeyValueStore::erase(const std::string& key) {
		std::unique_lock<std::shared_mutex> lock(mutex_);

		return data_.erase(key);
		//自动判断存在性：调用 erase(key) 时，容器会内部查找 key。
		//如果存在：删除该元素，返回 1（size_type 类型）。
		//如果不存在：不做任何事，返回 0。
	}

	std::vector<std::string> KeyValueStore::keys() const {
		std::shared_lock<std::shared_mutex> lock(mutex_);
		std::vector<std::string> ans;
		ans.reserve(data_.size());
		//现代C++习惯：
		for (const auto& [key, _] : data_) {
			ans.push_back(key);
		}

		//也可以用迭代器
		//for (auto it = data_.begin(); it != data_.end(); it++) {
		//	ans.push_back(it->first);
		//}
		std::sort(ans.begin(), ans.end());
		return ans;
	}

	std::size_t KeyValueStore::size() const {
		std::shared_lock<std::shared_mutex> lock(mutex_);
		return data_.size();
	}
}
