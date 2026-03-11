#ifndef TASKS_TIMER_H
#define TASKS_TIMER_H

#include <vector>
#include <chrono>

using namespace std::chrono;

class TaskTimer {
private:
    high_resolution_clock::time_point start_time;
    high_resolution_clock::time_point end_time;

public:
    void start();
    void stop();
    long long getDuration();
    long long measureSorting(std::vector<int>& data);
};

void task4();
void task5();

#endif