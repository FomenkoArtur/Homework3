#pragma once

#include <map>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>

template<typename T>
class PriorityQueue {
public:
    void push(const T& value, int priority) {
        std::unique_lock<std::mutex> lk(mutex_);
        // Используем отрицательный приоритет, чтобы большее число имело больший приоритет
        queue_.emplace(-priority, value);
        std::cout << "[PQ] thread " << std::this_thread::get_id()
                  << " push value with priority " << priority << "\n";
        lk.unlock();
        cv_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lk(mutex_);
        cv_.wait(lk, [&]() { return !queue_.empty(); });
        auto it = queue_.begin(); // минимальный (-priority) -> максимальный priority
        T value = it->second;
        queue_.erase(it);
        std::cout << "[PQ] thread " << std::this_thread::get_id()
                  << " pop value\n";
        return value;
    }

private:
    std::multimap<int, T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
};

