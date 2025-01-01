#ifndef LOOM_INTER_THREAD_COMMUNICATION_H
#define LOOM_INTER_THREAD_COMMUNICATION_H

#include "queue.h"
#include <set>

namespace loom
{

// forward declaration
template<typename T>
class Transmitter;

template<typename T>
class Receiver
{
public:
    Receiver() = default;

private:
    Queue<T> mQueue;

    friend class Transmitter<T>;
};

template<typename T>
class Transmitter
{
public:
    Transmitter() = default;

    void link(Receiver<T> &receiver)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mReceivers.insert(receiver);
    }

    void unlink(Receiver<T> &receiver)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mReceivers.erase(receiver);
    }

    void transmit(const T &data)
    {
        for (const auto &receiver: mReceivers)
        {
            receiver.mQueue.push(data);
        }
    }

private:
    std::mutex mMutex;
    std::set<std::reference_wrapper<Receiver<T>>> mReceivers;
};

}

#endif
