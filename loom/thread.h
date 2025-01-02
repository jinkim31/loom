#ifndef LOOM_THREAD_H
#define LOOM_THREAD_H

#include "itc.h"
#include <iostream>
#include <thread>
#include <functional>


namespace loom
{

class Thread
{
public:
    Thread();
    virtual ~Thread()= default;
    void start();
    void stop();

protected:
    template<typename T>
    typename Transmitter<T>::SharedPtr makeTransmitter();

    template<typename T>
    typename Receiver<T>::SharedPtr makeReceiver(const std::function<void(const T& data)>& callback);  // lambda version

    template<typename T, typename ThreadObjectType>
    typename Receiver<T>::SharedPtr makeReceiver(ThreadObjectType* object, void (ThreadObjectType::*callbackPtr)(const T& data));  // member function version

    virtual void step(){}

private:
    static void* entryPoint(void *param);
    void runEventLoop();

    std::chrono::high_resolution_clock::duration mEventLoopDelay;
    std::mutex mMutex;
    std::atomic<bool> mIsThreadRunning, mEventLoopBreakFlag;
    std::thread mThread;
    std::vector<std::shared_ptr<ReceiverInterface>> mReceivers;
};

template<typename T>
typename Transmitter<T>::SharedPtr Thread::makeTransmitter()
{
    return std::make_shared<Transmitter<T>>();
}

template<typename T>
typename Receiver<T>::SharedPtr Thread::makeReceiver(const std::function<void(const T& data)>& callback)
{
    auto receiver = std::make_shared<Receiver<T>>(callback);
    mReceivers.push_back(receiver);
    return receiver;
}

template<typename T, typename ThreadObjectType>
typename Receiver<T>::SharedPtr Thread::makeReceiver(ThreadObjectType *object, void (ThreadObjectType::*callbackPtr)(const T & data))
{
    auto receiver = std::make_shared<Receiver<T>>(object, callbackPtr);
    mReceivers.push_back(receiver);
    return receiver;
}


}

#endif
