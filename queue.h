#ifndef LOOM_QUEUE_H
#define LOOM_QUEUE_H

#include <queue>
#include <shared_mutex>

template<typename T>
class Queue
{
public:
    Queue()= default;
    void push(const T& data); // the data is passed by reference, pushed by copy
    T pop();    // the data is moved, move returned
    T top();    // the data is copied, move returned
    size_t len();
private:
    std::queue<T> mQueue;
    std::shared_mutex mMutex;
};

template<typename T>
void Queue<T>::push(const T& data)
{
    std::unique_lock<std::shared_mutex> lock(mMutex);
    mQueue.push(data); // copy (not move) for thread safety
}

template<typename T>
T Queue<T>::pop()
{
    std::unique_lock<std::shared_mutex> lock(mMutex);
    T data = std::move(mQueue.front());
    mQueue.pop();
    return data; // moved
}

template<typename T>
T Queue<T>::top()
{
    std::shared_lock<std::shared_mutex> lock(mMutex);
    return mQueue.front();
}

template<typename T>
size_t Queue<T>::len()
{
    std::shared_lock<std::shared_mutex> lock(mMutex);
    return mQueue.size();
}

#endif
