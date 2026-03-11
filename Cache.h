#pragma once

#include <map>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>

template<typename Key, typename Value>
class Cache {
public:
    inline void set(const Key& key, const Value& value) {
        {
            std::lock_guard<std::mutex> lk(mutex_);
            data_[key] = value;
        }
        std::cout << "[Cache] thread " << std::this_thread::get_id()
                  << " set key\n";
        cv_.notify_all();
    }

    inline Value get(const Key& key) {
        std::unique_lock<std::mutex> lk(mutex_);
        cv_.wait(lk, [&]() { return data_.find(key) != data_.end(); });
        Value value = data_[key];
        std::cout << "[Cache] thread " << std::this_thread::get_id()
                  << " get key\n";
        return value;
    }

    void print_all() {
        std::lock_guard<std::mutex> lk(mutex_);
        std::cout << "[Cache] contents:\n";
        for (const auto& [k, v] : data_) {
            std::cout << "  key -> value\n";
        }
    }

private:
    std::map<Key, Value> data_;
    std::mutex mutex_;
    std::condition_variable cv_;
};

