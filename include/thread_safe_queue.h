#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "sample.h"

class ThreadSafeQueue {
public:
    ThreadSafeQueue();

    void push(const Sample& sample);

    // Returns true if a sample was retrieved, false if queue is shut down
    bool pop(Sample& out);

    void shutdown();

private:
    std::queue<Sample>      queue_;
    std::mutex              mutex_;
    std::condition_variable cv_;
    bool                    done_;
};

#endif // THREAD_SAFE_QUEUE_H
