#include "tasks_basic.h"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

void task1() {
    cout << "\n=== ЗАДАЧА 1: Измерение времени выполнения ===" << endl;
    long long N = 100000000;
    cout << "Вычисление суммы от 1 до " << N << "..." << endl;

    auto start = high_resolution_clock::now();
    long long sum = 0;
    for (long long i = 1; i <= N; ++i) {
        sum += i;
    }
    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Время выполнения: " << duration.count() << " миллисекунд" << endl;
    cout << "Результат суммы: " << sum << endl;
}

void task2() {
    cout << "\n=== ЗАДАЧА 2: Таймер обратного отсчёта ===" << endl;
    int N;
    cout << "Введите количество секунд: ";
    if (!(cin >> N)) return;

    for (int i = N; i > 0; --i) {
        cout << "Осталось: " << i << " секунд" << endl;
        this_thread::sleep_for(1s);
    }
    cout << "Время вышло!" << endl;
}

void task3() {
    cout << "\n=== ЗАДАЧА 3: Конвертер времени ===" << endl;
    long long total_seconds;
    cout << "Введите количество секунд: ";
    if (!(cin >> total_seconds)) return;

    long long hours = total_seconds / 3600;
    long long minutes = (total_seconds % 3600) / 60;
    long long seconds = total_seconds % 60;

    auto declension = [](long long n, const char* f1, const char* f2, const char* f5) {
        n %= 100;
        if (n >= 11 && n <= 19) return f5;
        n %= 10;
        if (n == 1) return f1;
        if (n >= 2 && n <= 4) return f2;
        return f5;
    };

    cout << hours << " " << declension(hours, "час", "часа", "часов") << " "
         << minutes << " " << declension(minutes, "минута", "минуты", "минут") << " "
         << seconds << " " << declension(seconds, "секунда", "секунды", "секунд") << endl;
}