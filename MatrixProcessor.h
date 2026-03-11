#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <iostream>

template<typename T>
class MatrixProcessor {
public:
    MatrixProcessor(std::vector<std::vector<T>>& matrix, std::size_t n_threads)
            : matrix_(matrix),
              n_threads_(n_threads == 0 ? 1 : n_threads),
              completed_(0) {}

    void apply(const std::function<T(T)>& func) {
        std::size_t rows = matrix_.size();
        if (rows == 0) return;
        std::size_t cols = matrix_[0].size();
        std::size_t total = rows * cols;
        std::size_t block = (total + n_threads_ - 1) / n_threads_;

        auto worker = [this, &func, cols](std::size_t begin, std::size_t end) {
            std::cout << "[Matrix] thread " << std::this_thread::get_id()
                      << " start segment\n";
            for (std::size_t idx = begin; idx < end; ++idx) {
                std::size_t r = idx / cols;
                std::size_t c = idx % cols;
                matrix_[r][c] = func(matrix_[r][c]);
                if ((idx - begin) % 128 == 0) {
                    std::this_thread::yield();
                }
            }
            std::cout << "[Matrix] thread " << std::this_thread::get_id()
                      << " end segment\n";
            {
                std::lock_guard<std::mutex> lk(mutex_done_);
                ++completed_;
            }
            cv_done_.notify_one();
        };

        launched_threads_ = 0;
        for (std::size_t i = 0; i < n_threads_; ++i) {
            std::size_t begin = i * block;
            if (begin >= total) break;
            std::size_t end = std::min(begin + block, total);
            ++launched_threads_;
            std::thread([=]() { worker(begin, end); }).detach();
        }

        {
            std::unique_lock<std::mutex> lk(mutex_done_);
            cv_done_.wait(lk, [this]() { return completed_ >= launched_threads_; });
        }
    }

private:
    std::vector<std::vector<T>>& matrix_;
    std::size_t n_threads_;

    std::mutex mutex_done_;
    std::condition_variable cv_done_;
    std::size_t completed_;
    std::size_t launched_threads_{0};
};

