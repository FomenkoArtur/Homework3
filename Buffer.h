#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>

template<typename T>
class Buffer {
public:
    explicit Buffer(std::size_t capacity)
            : capacity_(capacity) {}

    void produce(const T& value) {
        std::unique_lock<std::mutex> lk(mutex_);
        not_full_.wait(lk, [&]() { return buffer_.size() < capacity_; });

        buffer_.push_back(value);
        std::cout << "[Buffer] producer " << std::this_thread::get_id()
                  << " produced " << value << "\n";
        lk.unlock();
        not_empty_.notify_one();
    }

    T consume() {
        std::unique_lock<std::mutex> lk(mutex_);
        not_empty_.wait(lk, [&]() { return !buffer_.empty(); });

        T value = buffer_.front();
        buffer_.erase(buffer_.begin());
        std::cout << "[Buffer] consumer " << std::this_thread::get_id()
                  << " consumed " << value << "\n";
        lk.unlock();
        not_full_.notify_one();
        return value;
    }

private:
    std::vector<T> buffer_;
    std::size_t capacity_;
    std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
};

