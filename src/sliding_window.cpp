#include "sliding_window.h"
#include <cmath>   // log()

SlidingWindow::SlidingWindow(int capacity)
    : capacity_(capacity),
      size_(0),
      head_(0),
      buffer_(capacity, 0.0),
      sum_(0.0),
      sum_sq_(0.0),
      sum_log_(0.0),
      zero_count_(0)
{}

void SlidingWindow::push(double value) {
    // If window is full, evict the oldest value first
    if (size_ == capacity_) {
        double old = buffer_[head_];
        sum_    -= old;
        sum_sq_ -= old * old;
        if (old == 0.0) {
            zero_count_--;
        } else {
            sum_log_ -= std::log(old);
        }
    } else {
        size_++;
    }

    // Write new value at head_ position
    buffer_[head_] = value;
    head_ = (head_ + 1) % capacity_;

    // Update running sums
    sum_    += value;
    sum_sq_ += value * value;
    if (value == 0.0) {
        zero_count_++;
    } else {
        sum_log_ += std::log(value);
    }
}

bool   SlidingWindow::full()       const { return size_ == capacity_; }
int    SlidingWindow::size()       const { return size_; }
int    SlidingWindow::capacity()   const { return capacity_; }
double SlidingWindow::sum()        const { return sum_; }
double SlidingWindow::sum_sq()     const { return sum_sq_; }
double SlidingWindow::sum_log()    const { return sum_log_; }
int    SlidingWindow::zero_count() const { return zero_count_; }
