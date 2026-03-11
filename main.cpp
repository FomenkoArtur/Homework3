#include <iostream>
<<<<<<< HEAD
#include <locale>

#ifdef _WIN32
#include <windows.h>
#endif

#include "tasks_basic.h"
#include "tasks_timer.h"
#include "tasks_hyperthreading.h"

using namespace std;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "Russian");
#endif

    cout << "Выберите задачу для выполнения (1-8) или 0 для выхода:" << endl;

    int choice;
    while (true) {
        cout << "\nВведите номер задачи: ";
        if (!(cin >> choice)) break;
        if (choice == 0) break;

        switch (choice) {
            case 1: task1(); break;
            case 2: task2(); break;
            case 3: task3(); break;
            case 4: task4(); break;
            case 5: task5(); break;
            case 6: task6(); break;
            case 7: task7(); break;
            case 8: task8(); break;
            default: cout << "Неверный номер задачи." << endl;
        }
    }

    return 0;
}
=======
#include <vector>
#include <thread>
#include <chrono>

#include "ParallelSum.h"
#include "Bank.h"
#include "Buffer.h"
#include "Logger.h"
#include "Cache.h"
#include "MatrixProcessor.h"
#include "PriorityQueue.h"

int main() {
    // Task 1 demo
    std::vector<int> data(1000);
    for (int i = 0; i < 1000; ++i) data[i] = i + 1;
    ParallelSum<int> ps(data, 4);
    std::cout << "Parallel sum = " << ps.compute_sum() << "\n";

    // Task 2 demo
    Bank<int> bank({1000, 1000, 1000});
    std::vector<std::thread> client_threads;
    for (int i = 0; i < 5; ++i) {
        client_threads.emplace_back([&bank, i]() {
            int from = i % 3;
            int to = (i + 1) % 3;
            bank.transfer(from, to, 100);
        });
    }
    for (auto& t : client_threads) t.join();

    // Task 3 demo
    Buffer<int> buffer(5);
    std::thread producer([&buffer]() {
        for (int i = 0; i < 10; ++i) {
            buffer.produce(i);
            std::this_thread::yield();
        }
    });
    std::thread consumer([&buffer]() {
        for (int i = 0; i < 10; ++i) {
            buffer.consume();
            std::this_thread::yield();
        }
    });
    producer.join();
    consumer.join();

    // Task 4 demo
    Logger<std::string> logger("log.txt");
    std::thread tlog1([&logger]() { logger.log("Message from thread 1"); });
    std::thread tlog2([&logger]() { logger.log("Message from thread 2"); });
    tlog1.join();
    tlog2.join();

    // Task 5 demo
    Cache<int, std::string> cache;
    std::thread setter([&cache]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cache.set(1, "one");
    });
    std::thread getter([&cache]() {
        auto v = cache.get(1);
        std::cout << "Cache get returned: " << v << "\n";
    });
    setter.join();
    getter.join();

    // Task 6 demo
    std::vector<std::vector<int>> matrix(5, std::vector<int>(5, 2));
    MatrixProcessor<int> mp(matrix, 3);
    mp.apply([](int x) { return x * 2; });

    // Task 7 demo
    PriorityQueue<std::string> pq;
    std::thread prod_pq([&pq]() {
        pq.push("low", 1);
        pq.push("high", 10);
    });
    std::thread cons_pq([&pq]() {
        auto v1 = pq.pop();
        auto v2 = pq.pop();
        std::cout << "PQ popped: " << v1 << ", " << v2 << "\n";
    });
    prod_pq.join();
    cons_pq.join();

    return 0;
}

>>>>>>> 802bc48 (Add multithreading tasks solution)
