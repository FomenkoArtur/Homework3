#ifndef TASKS_HYPERTHREADING_H
#define TASKS_HYPERTHREADING_H

#include <vector>
#include <queue>
#include <string>
#include <chrono>

using namespace std::chrono;

class Task {
public:
    int value;
    int priority;
    int duration_ms;
    int steps;
    int currentStep;

    Task(int v, int p, int d, int s);
    bool operator<(const Task& other) const;
};

class VirtualThread {
private:
    std::priority_queue<Task> tasks;
    int id;

public:
    VirtualThread(int tid);
    void addTask(const Task& t);
    bool isEmpty() const;
    void runStep(int threadId);
};

class HyperThreadingSimulator {
private:
    VirtualThread vt1;
    VirtualThread vt2;

public:
    HyperThreadingSimulator();
    void execute();

    void addTask(int threadId, int v, int p, int d, int s);
};

void task6();
void task7();
void task8();

#endif