#include "aggregator.h"
#include "fusion1.h"
#include "fusion2.h"
#include <iostream>
#include <chrono>
#include <thread>

// Sensor frequencies in Hz
static const double SENSOR_FREQS[4] = { 96000.0, 71000.0, 69900.0, 23000.0 };

Aggregator::Aggregator(int window_size, int run_seconds)
    : window_size_(window_size),
      run_seconds_(run_seconds),
      stop_flag_(false),
      window_(window_size)
{}

Aggregator::~Aggregator() {
    for (int i = 0; i < static_cast<int>(simulators_.size()); i++) {
        delete simulators_[i];
    }
}

void Aggregator::run() {
    // Create and launch 4 sensor producer threads
    for (int i = 0; i < 4; i++) {
        SensorSimulator* sim = new SensorSimulator(i, SENSOR_FREQS[i],
                                                   &queue_, &stop_flag_);
        simulators_.push_back(sim);
        threads_.push_back(std::thread(&SensorSimulator::run, sim));
    }

    // Launch consumer thread
    threads_.push_back(std::thread(&Aggregator::consume, this));

    // Run for the configured duration then stop
    std::this_thread::sleep_for(std::chrono::seconds(run_seconds_));
    stop_flag_.store(true);
    queue_.shutdown();

    // Join all threads
    for (int i = 0; i < static_cast<int>(threads_.size()); i++) {
        threads_[i].join();
    }

    std::cout << "Aggregator stopped after " << run_seconds_ << " seconds." << std::endl;
}

void Aggregator::consume() {
    Sample s;
    long long sample_count = 0;
    const int print_every  = 10000; // rate-limit output

    while (queue_.pop(s)) {
        window_.push(s.value);
        sample_count++;

        if (window_.full() && sample_count % print_every == 0) {
            double f1 = compute_fusion1(window_);
            double f2 = compute_fusion2(window_);
            std::cout << "samples=" << sample_count
                      << "  F1=" << f1
                      << "  F2=" << f2
                      << std::endl;
        }
    }
}
