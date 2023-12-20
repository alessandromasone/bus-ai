//
//  semaphore.h
//  Bus
//
//  Created by Alessandro masone on 21/04/22.
//

#ifndef semaphore_h
#define semaphore_h

class semaphore {
    mutex mutex_;
    condition_variable condition_;
    unsigned long count_ = 0; // Initialized as locked.
    unsigned long total_ = 0;
public:
    void release() {
        lock_guard<decltype(mutex_)> lock(mutex_);
        ++count_;
        condition_.notify_one();
    }
    void acquire() {
        unique_lock<decltype(mutex_)> lock(mutex_);
        while(!count_) // Handle spurious wake-ups.
            condition_.wait(lock);
        --count_;
    }
    bool try_acquire() {
        lock_guard<decltype(mutex_)> lock(mutex_);
        if(count_) {
            --count_;
            return true;
        }
        return false;
    }
    unsigned long value() { return count_; }
    void set(int value) { count_ = value; }
    unsigned long size() { return total_; }
    semaphore() { }
    ~semaphore() { }
};

#endif /* semaphore_h */
