#include "fusion2.h"
#include <cmath>  // sqrt()

double compute_fusion2(const SlidingWindow& window) {
    if (window.size() < 2) {
        return 0.0;
    }

    double n          = static_cast<double>(window.size());
    double sum        = window.sum();
    double sum_sq     = window.sum_sq();
    double pair_count = n * (n - 1.0);

    // Algebraic identity: sum of all pairwise products (i != j)
    double pairwise_sum = sum * sum - sum_sq;

    // Guard against tiny negative values from floating-point rounding
    if (pairwise_sum < 0.0) {
        pairwise_sum = 0.0;
    }

    return std::sqrt(pairwise_sum / pair_count);
}
