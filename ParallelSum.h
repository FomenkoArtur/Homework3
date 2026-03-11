#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <type_traits>

template<typename T>
class ParallelSum {
    static_assert(std::is_arithmetic<T>::value, "ParallelSum requires arithmetic type");

public:
    ParallelSum(const std::vector<T>& data, std::size_t n_threads)
            : data_(data),
              n_threads_(n_threads == 0 ? 1 : n_threads),
              completed_(0),
              total_sum_(0),
              launched_threads_(0) {}

    T compute_sum() {
        std::size_t n = data_.size();
        if (n == 0) {
            return T{};
        }

        std::size_t block = (n + n_threads_ - 1) / n_threads_;

        auto worker = [this](std::size_t begin, std::size_t end) {
            inline_sum_segment(begin, end);
            {
                std::lock_guard<std::mutex> lk(mutex_done_);
                ++completed_;
            }
            cv_done_.notify_one();
        };

        for (std::size_t i = 0; i < n_threads_; ++i) {
            std::size_t begin = i * block;
            if (begin >= n) break;
            std::size_t end = std::min(begin + block, n);

            ++launched_threads_;
            std::thread([=]() { worker(begin, end); }).detach();
        }

        {
            std::unique_lock<std::mutex> lk(mutex_done_);
            cv_done_.wait(lk, [this]() { return completed_ >= launched_threads_; });
        }

        return total_sum_;
    }

private:
    inline void inline_sum_segment(std::size_t begin, std::size_t end) {
        T local_sum = 0;
        for (std::size_t i = begin; i < end; ++i) {
            local_sum += data_[i];
            if ((i - begin) % 128 == 0) {
                std::this_thread::yield();
            }
        }
        std::cout << "[ParallelSum] thread " << std::this_thread::get_id()
                  << " segment sum = " << local_sum << "\n";
        {
            std::lock_guard<std::mutex> lk(mutex_sum_);
            total_sum_ += local_sum;
        }
    }

    const std::vector<T>& data_;
    std::size_t n_threads_;

    std::mutex mutex_sum_;
    T total_sum_;

    std::mutex mutex_done_;
    std::condition_variable cv_done_;
    std::size_t completed_;
    std::size_t launched_threads_;
};

