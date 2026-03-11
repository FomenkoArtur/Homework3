#pragma once

#include <fstream>
#include <mutex>
#include <sstream>
#include <iostream>
#include <thread>
#include <string>

template<typename T>
class Logger {
public:
    explicit Logger(const std::string& filename) {
        log_file_.open(filename, std::ios::out | std::ios::app);
    }

    ~Logger() {
        if (log_file_.is_open()) {
            log_file_.close();
        }
    }

    void log(const T& message) {
        auto write = [this](const std::string& text) {
            std::lock_guard<std::mutex> lk(mutex_);
            if (log_file_.is_open()) {
                log_file_ << text << std::endl;
            }
            std::cout << "[Logger] thread " << std::this_thread::get_id()
                      << " " << text << std::endl;
        };

        std::ostringstream oss;
        oss << message;
        write(oss.str());
    }

private:
    std::ofstream log_file_;
    std::mutex mutex_;
};

