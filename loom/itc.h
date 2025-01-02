#ifndef LOOM_ITC_H
#define LOOM_ITC_H

#include "queue.h"
#include <set>
#include <iostream>


namespace loom
{

// forward declaration
template<typename T>
class Transmitter;
class Thread;

// receiver
class ReceiverInterface
{
protected:
    virtual void receive()=0;
    friend class Thread;
};

template<typename T>
class Receiver : public ReceiverInterface
{
public:
    Receiver(const std::function<void(const T &)> &callback); // lambda version
    template<typename ThreadObjectType>
    Receiver(ThreadObjectType* object, void (ThreadObjectType::*callbackPtr)(const T& data));
    using SharedPtr = std::shared_ptr<Receiver<T>>;
private:
    void receive() override;
    Queue<T> mQueue;
    std::function<void(const T &)> mCallback;
    friend class Transmitter<T>;
};

template<typename T>
void Receiver<T>::receive()
{
    for(int i=0; i<mQueue.len(); i++)
        mCallback(mQueue.pop());
}

template<typename T>
Receiver<T>::Receiver(const std::function<void(const T &)> &callback)
{
    mCallback = callback;
}

template<typename T>
template<typename ThreadObjectType>
Receiver<T>::Receiver(ThreadObjectType* object, void (ThreadObjectType::*callbackPtr)(const T& data))
{
    mCallback = [=](const T& data){(object->*callbackPtr)(data);};
}

// transmitter
template<typename T>
class Transmitter
{
public:
    Transmitter()= default;
    ~Transmitter();
    void link(std::shared_ptr<Receiver<T>> receiver);
    void unlink(std::shared_ptr<Receiver<T>> receiver);
    void transmit(const T &data);
    using SharedPtr = std::shared_ptr<Transmitter<T>>;

private:
    std::mutex mMutex;
    std::set<std::shared_ptr<Receiver<T>>> mReceivers;
};

template<typename T>
void Transmitter<T>::link(std::shared_ptr<Receiver<T>> receiver)
{
    std::unique_lock<std::mutex> lock(mMutex);
    mReceivers.insert(receiver);
}

template<typename T>
void Transmitter<T>::unlink(std::shared_ptr<Receiver<T>> receiver)
{
    std::unique_lock<std::mutex> lock(mMutex);
    mReceivers.erase(receiver);
}

template<typename T>
void Transmitter<T>::transmit(const T &data)
{
    std::unique_lock<std::mutex> lock(mMutex);
    for (const auto &receiver: mReceivers)
    {
        receiver->mQueue.push(data);
    }
}

template<typename T>
Transmitter<T>::~Transmitter()
{
    if(!mReceivers.empty())
        std::cerr<<"[ Loom ] Receiver(s) remain linked when Transmitter is destructed."
                   " Ensure to unlink using Transmitter->unlink(Receiver<T>) before destruction."<<std::endl;
}

}

#endif
