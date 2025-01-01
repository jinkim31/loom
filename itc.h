#ifndef LOOM_ITC_H
#define LOOM_ITC_H

#include "queue.h"
#include <set>
#include <iostream>


namespace loom
{

// forward declaration
template<typename T>
class TransmitterImpl;
class Thread;

// receiver
class ReceiverInterface
{
protected:
    virtual void receive()=0;
    friend class Thread;
};

template<typename T>
class ReceiverImpl : public ReceiverInterface
{
public:
    ReceiverImpl(const std::function<void(const T &)> &callback); // lambda version
    template<typename ThreadObjectType>
    ReceiverImpl(ThreadObjectType* object, void (ThreadObjectType::*callbackPtr)(const T& data));

private:
    void receive() override;
    Queue<T> mQueue;
    std::function<void(const T &)> mCallback;
    friend class TransmitterImpl<T>;
};

template<typename T>
void ReceiverImpl<T>::receive()
{
    for(int i=0; i<mQueue.len(); i++)
        mCallback(mQueue.pop());
}

template<typename T>
ReceiverImpl<T>::ReceiverImpl(const std::function<void(const T &)> &callback)
{
    mCallback = callback;
}

template<typename T>
template<typename ThreadObjectType>
ReceiverImpl<T>::ReceiverImpl(ThreadObjectType* object, void (ThreadObjectType::*callbackPtr)(const T& data))
{
    mCallback = [=](const T& data){(object->*callbackPtr)(data);};
}

// transmitter
template<typename T>
class TransmitterImpl
{
public:
    TransmitterImpl()= default;
    ~TransmitterImpl();
    void link(std::shared_ptr<ReceiverImpl<T>> receiver);
    void unlink(std::shared_ptr<ReceiverImpl<T>> receiver);
    void transmit(const T &data);

private:
    std::mutex mMutex;
    std::set<std::shared_ptr<ReceiverImpl<T>>> mReceivers;
};

template<typename T>
void TransmitterImpl<T>::link(std::shared_ptr<ReceiverImpl<T>> receiver)
{
    std::unique_lock<std::mutex> lock(mMutex);
    mReceivers.insert(receiver);
}

template<typename T>
void TransmitterImpl<T>::unlink(std::shared_ptr<ReceiverImpl<T>> receiver)
{
    std::unique_lock<std::mutex> lock(mMutex);
    mReceivers.erase(receiver);
}

template<typename T>
void TransmitterImpl<T>::transmit(const T &data)
{
    std::unique_lock<std::mutex> lock(mMutex);
    for (const auto &receiver: mReceivers)
    {
        receiver->mQueue.push(data);
    }
}

template<typename T>
TransmitterImpl<T>::~TransmitterImpl()
{
    if(!mReceivers.empty())
        std::cerr<<"[ Loom ] Receiver(s) remain linked when Transmitter is destructed."
                   " Ensure to unlink using Transmitter->unlink(Receiver<T>) before destruction."<<std::endl;
}

template <typename T>
using Transmitter = std::shared_ptr<TransmitterImpl<T>> ;

template <typename T>
using Receiver = std::shared_ptr<ReceiverImpl<T>>;

}

#endif
