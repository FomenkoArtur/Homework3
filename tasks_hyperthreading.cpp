#include "tasks_hyperthreading.h"
#include <iostream>
#include <random>
#include <thread>

using namespace std;

class VirtualThreadFact {
private:
    vector<int> tasks;
    int index = 0;
    int id;

    long long factorial(int n) {
        long long res = 1;
        for (int i = 2; i <= n; ++i) res *= i;
        return res;
    }

public:
    VirtualThreadFact(int tid, vector<int> t) : id(tid), tasks(t) {}

    bool run() {
        if (index >= tasks.size()) return false;
        int val = tasks[index++];
        cout << "Виртуальный поток " << id << " вычисляет " << val << "! = " << factorial(val) << endl;
        return true;
    }

    bool hasTasks() const { return index < tasks.size(); }
};

class HyperThreadingSimulatorFact {
private:
    VirtualThreadFact vt1;
    VirtualThreadFact vt2;

public:
    HyperThreadingSimulatorFact()
        : vt1(1, {5, 10}), vt2(2, {7, 12}) {}

    void execute() {
        bool t1HasTasks = true;
        bool t2HasTasks = true;

        while (t1HasTasks || t2HasTasks) {
            if (t1HasTasks) t1HasTasks = vt1.run();
            if (t2HasTasks) t2HasTasks = vt2.run();
        }
    }
};

void task6() {
    cout << "\n=== ЗАДАЧА 6: Hyper-Threading (Факториалы) ===" << endl;
    HyperThreadingSimulatorFact simulator;
    simulator.execute();
}

class VirtualThreadTask {
private:
    vector<string> queue;
    int index = 0;
    int id;
    enum State { IDLE, STARTED };
    State state = IDLE;
    string currentTaskName;

public:
    VirtualThreadTask(int tid, vector<string> q) : id(tid), queue(q) {}

    bool runNextTask() {
        if (state == IDLE) {
            if (index >= queue.size()) return false;
            currentTaskName = queue[index];
            cout << "Виртуальный поток " << id << " начал " << currentTaskName << endl;
            state = STARTED;
            return true;
        } else {
            cout << "Виртуальный поток " << id << " закончил " << currentTaskName << endl;
            state = IDLE;
            index++;
            return true;
        }
    }

    bool hasTasks() const { return index < queue.size() || state == STARTED; }
};

class HyperThreadingSimulatorTask {
private:
    VirtualThreadTask vt1;
    VirtualThreadTask vt2;

public:
    HyperThreadingSimulatorTask()
        : vt1(1, {"Задачу A", "Задачу C"}), vt2(2, {"Задачу B", "Задачу D"}) {}

    void execute() {
        while (vt1.hasTasks() || vt2.hasTasks()) {
            if (vt1.hasTasks()) vt1.runNextTask();
            if (vt2.hasTasks()) vt2.runNextTask();
        }
    }
};

void task7() {
    cout << "\n=== ЗАДАЧА 7: Hyper-Threading (Очередь заданий) ===" << endl;
    HyperThreadingSimulatorTask simulator;
    simulator.execute();
}

Task::Task(int v, int p, int d, int s)
    : value(v), priority(p), duration_ms(d), steps(s), currentStep(0) {}

bool Task::operator<(const Task& other) const {
    return priority < other.priority;
}

VirtualThread::VirtualThread(int tid) : id(tid) {}

void VirtualThread::addTask(const Task& t) {
    tasks.push(t);
}

bool VirtualThread::isEmpty() const {
    return tasks.empty();
}

void VirtualThread::runStep(int threadId) {
    if (tasks.empty()) return;

    Task task = tasks.top();
    tasks.pop();

    task.currentStep++;

    int sleepTime = task.duration_ms / task.steps;

    cout << "Виртуальный поток " << threadId << " выполняет шаг "
         << task.currentStep << "/" << task.steps
         << " задачи " << task.value
         << " с приоритетом " << task.priority << endl;

    if (task.currentStep >= task.steps) {
        int result = static_cast<int>(task.value * task.value);
        cout << "Виртуальный поток " << threadId << " завершил задачу "
             << task.value << ": результат = " << result << endl;
    } else {
        tasks.push(task);
    }
}

HyperThreadingSimulator::HyperThreadingSimulator() : vt1(1), vt2(2) {}

void HyperThreadingSimulator::addTask(int threadId, int v, int p, int d, int s) {
    if (threadId == 1) {
        vt1.addTask(Task(v, p, d, s));
    } else {
        vt2.addTask(Task(v, p, d, s));
    }
}

void HyperThreadingSimulator::execute() {
    while (!vt1.isEmpty() || !vt2.isEmpty()) {
        if (!vt1.isEmpty()) vt1.runStep(1);
        if (!vt2.isEmpty()) vt2.runStep(2);
    }
}

void task8() {
    cout << "\n=== ЗАДАЧА 8: Продвинутый Hyper-Threading ===" << endl;
    HyperThreadingSimulator simulator;

    mt19937 gen(random_device{}());
    uniform_int_distribution<> distVal(1, 50);
    uniform_int_distribution<> distDur(200, 1000);
    uniform_int_distribution<> distPri(1, 10);
    uniform_int_distribution<> distSteps(2, 5);

    cout << "Генерация случайных задач..." << endl;
    for (int i = 0; i < 5; ++i) {
        int v = distVal(gen);
        int d = distDur(gen);
        int p = distPri(gen);
        int s = distSteps(gen);
        if (i % 2 == 0)
            simulator.addTask(1, v, p, d, s);
        else
            simulator.addTask(2, v, p, d, s);
    }

    cout << "Начало выполнения..." << endl;
    simulator.execute();
}