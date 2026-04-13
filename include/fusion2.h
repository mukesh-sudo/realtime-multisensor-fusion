#ifndef FUSION2_H
#define FUSION2_H

#include "sliding_window.h"

// Function 2: Square root of average pairwise products
// Uses identity: sum_{i!=j} xi*xj = sum^2 - sum_sq
double compute_fusion2(const SlidingWindow& window);

#endif // FUSION2_H
