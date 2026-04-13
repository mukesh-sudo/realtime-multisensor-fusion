#include "sensor_simulator.h"
#include <chrono>
#include <thread>
#include <cstdlib>  // rand(), RAND_MAX

SensorSimulator::SensorSimulator(int sensor_id, double freq_hz,
                                 ThreadSafeQueue* queue,
                                 std::atomic<bool>* stop_flag)
    : sensor_id_(sensor_id),
      freq_hz_(freq_hz),
      queue_(queue),
      stop_flag_(stop_flag),
      seed_(static_cast<unsigned int>(sensor_id + 1) * 1234567u)
{}

void SensorSimulator::run() {
    // interval in nanoseconds between samples
    long long interval_ns = static_cast<long long>(1.0e9 / freq_hz_);

    while (!stop_flag_->load()) {
        // Per-thread LCG — thread-safe alternative to rand() which uses global state
        seed_ = seed_ * 1103515245u + 12345u;
        double value = 100.0 * static_cast<double>(seed_ & 0x7FFFFFFFu)
                             / static_cast<double>(0x7FFFFFFFu);

        // Timestamp: nanoseconds from steady_clock (portable, explicit)
        uint64_t ts = static_cast<uint64_t>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now().time_since_epoch()
            ).count()
        );

        Sample s;
        s.value        = value;
        s.timestamp_ns = ts;
        s.sensor_id    = sensor_id_;

        queue_->push(s);

        std::this_thread::sleep_for(std::chrono::nanoseconds(interval_ns));
    }
}
