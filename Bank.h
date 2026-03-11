#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>
#include <iostream>

template<typename T>
class Account {
public:
    explicit Account(T balance = 0) : balance_(balance) {}

    T get_balance() const {
        std::lock_guard<std::mutex> lk(mutex_);
        return balance_;
    }

    void deposit(T amount) {
        std::lock_guard<std::mutex> lk(mutex_);
        balance_ += amount;
    }

    bool withdraw(T amount) {
        std::lock_guard<std::mutex> lk(mutex_);
        if (balance_ < amount) return false;
        balance_ -= amount;
        return true;
    }

    mutable std::mutex mutex_;

private:
    T balance_;
};

template<typename T>
class Bank {
public:
    explicit Bank(const std::vector<T>& balances) {
        for (T b : balances) {
            accounts_.emplace_back(b);
        }
    }

    inline void transfer(int from, int to, T amount) {
        if (from == to) return;
        if (from < 0 || to < 0 ||
            from >= static_cast<int>(accounts_.size()) ||
            to >= static_cast<int>(accounts_.size())) {
            return;
        }

        Account<T>& acc_from = accounts_[from];
        Account<T>& acc_to = accounts_[to];

        std::unique_lock<std::mutex> lk_from(acc_from.mutex_);
        cv_.wait(lk_from, [&]() { return acc_from.get_balance() >= amount; });

        auto do_transfer = [&]() {
            acc_from.withdraw(amount);
            {
                std::lock_guard<std::mutex> lk_to(acc_to.mutex_);
                acc_to.deposit(amount);
            }
        };

        do_transfer();
        std::cout << "[Bank] thread " << std::this_thread::get_id()
                  << " transfer " << amount << " from " << from << " to " << to << "\n";

        lk_from.unlock();
        cv_.notify_all();
    }

    std::vector<Account<T>>& accounts() { return accounts_; }

private:
    std::vector<Account<T>> accounts_;
    std::condition_variable cv_;
};

