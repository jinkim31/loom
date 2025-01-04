#ifndef LOOM_THREADING_QUEUE_H
#define LOOM_THREADING_QUEUE_H

#include <queue>
#include <shared_mutex>
#include <iostream>

template<typename T>
class Queue
{
public:
    Queue()= default;
    void push(T&& data); // the data is passed using move, pushed by copy. data should be a copied instance
    T pop();    // the data is moved from std::queue, move returned
    size_t size();
    void clear();
private:
    std::queue<T> mQueue;
    std::shared_mutex mMutex;
};

template<typename T>
void Queue<T>::push(T&& data)
{
    std::unique_lock<std::shared_mutex> lock(mMutex);
    mQueue.push(std::move(data)); // data should be a copy of original
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
size_t Queue<T>::size()
{
    std::shared_lock<std::shared_mutex> lock(mMutex);
    return mQueue.size();
}

template<typename T>
void Queue<T>::clear()
{
    std::unique_lock<std::shared_mutex> lock(mMutex);
    while(!mQueue.empty()) mQueue.pop();
}

#endif
