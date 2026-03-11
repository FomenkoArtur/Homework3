#include "tasks_timer.h"
#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

void TaskTimer::start() {
    start_time = high_resolution_clock::now();
}

void TaskTimer::stop() {
    end_time = high_resolution_clock::now();
}

long long TaskTimer::getDuration() {
    return duration_cast<milliseconds>(end_time - start_time).count();
}

long long TaskTimer::measureSorting(vector<int>& data) {
    start();
    sort(data.begin(), data.end());
    stop();
    return getDuration();
}

void task4() {
    cout << "\n=== ЗАДАЧА 4: Класс TaskTimer ===" << endl;
    TaskTimer timer;
    int size = 100000;

    vector<int> data1(size);
    generate(data1.begin(), data1.end(), rand);
    long long time1 = timer.measureSorting(data1);
    cout << "Время выполнения сортировки: " << time1 << " миллисекунд" << endl;

    vector<int> data2(size);
    generate(data2.begin(), data2.end(), rand);
    long long time2 = timer.measureSorting(data2);
    cout << "Время выполнения сортировки: " << time2 << " миллисекунд" << endl;
}

// Сортировки для Задачи 5
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n - i - 1; ++j)
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
}

void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int i = low, j = high;
        int pivot = arr[(low + high) / 2];
        while (i <= j) {
            while (arr[i] < pivot) i++;
            while (arr[j] > pivot) j--;
            if (i <= j) {
                swap(arr[i], arr[j]);
                i++;
                j--;
            }
        }
        if (low < j) quickSort(arr, low, j);
        if (i < high) quickSort(arr, i, high);
    }
}

void task5() {
    cout << "\n=== ЗАДАЧА 5: Сравнение алгоритмов сортировки ===" << endl;
    int N = 100000;
    cout << "Генерация массива из " << N << " элементов..." << endl;
    
    vector<int> originalData(N);
    mt19937 gen(42);
    uniform_int_distribution<> dis(1, 10000);
    for(int& x : originalData) x = dis(gen);

    auto measure = [&](const string& name, auto sortFunc) {
        vector<int> data = originalData;
        auto start = high_resolution_clock::now();
        sortFunc(data);
        auto end = high_resolution_clock::now();
        auto dur = duration_cast<milliseconds>(end - start).count();
        cout << name << ": " << dur << " мс" << endl;
    };

    cout << "Запуск сортировок..." << endl;
    cout << "Пузырьковая сортировка: пропущена (слишком долго для 100к)" << endl;
    measure("Сортировка вставками", [](vector<int>& v){ insertionSort(v); });
    measure("Сортировка слиянием", [](vector<int>& v){ mergeSort(v, 0, v.size() - 1); });
    measure("Быстрая сортировка", [](vector<int>& v){ quickSort(v, 0, v.size() - 1); });
    measure("Стандартная std::sort", [](vector<int>& v){ sort(v.begin(), v.end()); });
}