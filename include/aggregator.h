#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include <thread>
#include <atomic>
#include "thread_safe_queue.h"
#include "sensor_simulator.h"
#include "sliding_window.h"

static const int NUM_SENSORS = 4;

class Aggregator {
public:
    Aggregator(int window_size, int run_seconds);
    ~Aggregator();

    void run();

private:
    void consume();

    int               window_size_;
    int               run_seconds_;

    ThreadSafeQueue   queue_;
    std::atomic<bool> stop_flag_;
    SlidingWindow     window_;

    SensorSimulator*  simulators_[NUM_SENSORS];
    std::thread       threads_[NUM_SENSORS + 1]; // 4 sensors + 1 consumer
};

#endif // AGGREGATOR_H
