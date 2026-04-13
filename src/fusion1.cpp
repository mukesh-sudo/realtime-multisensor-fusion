#include "fusion1.h"
#include <cmath>  // exp()

double compute_fusion1(const SlidingWindow& window) {
    if (window.size() == 0) {
        return 0.0;
    }

    // Any zero in the window makes the geometric mean zero
    if (window.zero_count() > 0) {
        return 0.0;
    }

    return std::exp(window.sum_log() / static_cast<double>(window.size()));
}
