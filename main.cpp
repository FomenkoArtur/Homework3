#include <iostream>
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
