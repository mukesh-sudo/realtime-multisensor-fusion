#ifndef SLIDING_WINDOW_H
#define SLIDING_WINDOW_H

#include <vector>
#include <cstddef>

class SlidingWindow {
public:
    explicit SlidingWindow(int capacity);

    void push(double value);

    bool   full()       const;
    int    size()       const;
    int    capacity()   const;

    double sum()        const;
    double sum_sq()     const;
    double sum_log()    const;
    int    zero_count() const;

private:
    int                 capacity_;
    int                 size_;
    int                 head_;      // index of oldest element
    std::vector<double> buffer_;

    double sum_;
    double sum_sq_;
    double sum_log_;
    int    zero_count_;
};

#endif // SLIDING_WINDOW_H
