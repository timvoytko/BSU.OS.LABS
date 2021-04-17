#ifndef BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL_H_

#include <algorithm>
#include <queue>
#include <condition_variable>
#include <mutex>
template<class T>
class BufferedChannel {
public:
    explicit BufferedChannel(int size) :max_size_(size) {}


    void Send(T value) {
        std::unique_lock<std::mutex> lock(this->buffer_mutex_);
        cv_.wait(lock, [&]() { return this->if_not_full(); });
        if (!this->is_open_) {
            throw std::runtime_error("Trying to send data via a closed channel");
        }
        this->buffer_.push(value);
        if (!this->if_not_full()) {
            this->is_open_ = false;
        }
        lock.unlock();
        cv_.notify_all();

    }
    std::pair<T, bool> Recv() {
        std::unique_lock<std::mutex> lock(this->buffer_mutex_);
        cv_.wait(lock, [&]() { return !this->is_empty(); });
        // T val{};
        T val = buffer_.front();
        buffer_.pop();
    
        lock.unlock();
        cv_.notify_all();
        return { val, is_open_ };
    }

    void Close() {
        std::unique_lock<std::mutex> lock(this->buffer_mutex_);
        this->is_open_ = false;
        lock.unlock();
        cv_.notify_all();

    }

    bool is_empty() {
        return buffer_.empty();
    }
    bool if_not_full() {
        return buffer_.size() < max_size_;
    }
    bool if_size_permissible() {
        return !this->is_empty() && this->if_not_full();
    }

private:
    std::queue<T> buffer_;
    std::condition_variable cv_;
    std::mutex buffer_mutex_; // controls access to buffer
    bool is_open_ = true;
    int max_size_;

};

#endif 