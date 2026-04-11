#ifndef SENSOR_SIMULATOR_H
#define SENSOR_SIMULATOR_H

#include <atomic>
#include <cstdint>
#include "thread_safe_queue.h"

class SensorSimulator {
public:
    // freq_hz: how many samples per second this sensor produces
    SensorSimulator(int sensor_id, double freq_hz,
                    ThreadSafeQueue* queue,
                    std::atomic<bool>* stop_flag);

    void run(); // called from std::thread

private:
    int                sensor_id_;
    double             freq_hz_;
    ThreadSafeQueue*   queue_;
    std::atomic<bool>* stop_flag_;
};

#endif // SENSOR_SIMULATOR_H
