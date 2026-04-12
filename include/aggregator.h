#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include <thread>
#include <atomic>
#include <vector>
#include "thread_safe_queue.h"
#include "sensor_simulator.h"
#include "sliding_window.h"

class Aggregator {
public:
    Aggregator(int window_size, int run_seconds);
    ~Aggregator();

    void run();

private:
    void consume();

    int                           window_size_;
    int                           run_seconds_;

    ThreadSafeQueue               queue_;
    std::atomic<bool>             stop_flag_;
    SlidingWindow                 window_;

    std::vector<SensorSimulator*> simulators_;
    std::vector<std::thread>      threads_;
};

#endif // AGGREGATOR_H
