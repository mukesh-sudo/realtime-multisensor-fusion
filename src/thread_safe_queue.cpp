#include "thread_safe_queue.h"

ThreadSafeQueue::ThreadSafeQueue() : done_(false) {}

void ThreadSafeQueue::push(const Sample& sample) {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(sample);
    }
    cv_.notify_one();
}

bool ThreadSafeQueue::pop(Sample& out) {
    std::unique_lock<std::mutex> lock(mutex_);

    while (queue_.empty() && !done_) {
        cv_.wait(lock);
    }

    if (queue_.empty()) {
        return false; // shutdown was called and queue is drained
    }

    out = queue_.front();
    queue_.pop();
    return true;
}

void ThreadSafeQueue::shutdown() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        done_ = true;
    }
    cv_.notify_all();
}
