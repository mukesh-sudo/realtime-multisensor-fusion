#ifndef SAMPLE_H
#define SAMPLE_H

#include <cstdint>

struct Sample {
    double   value;        // temperature reading [0.0, 100.0]
    uint64_t timestamp_ns; // nanoseconds since epoch (steady_clock)
    int      sensor_id;    // 0..3 identifying which sensor produced this
};

#endif // SAMPLE_H
