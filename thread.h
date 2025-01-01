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
    Transmitter<T> makeTransmitter();

    template<typename T>
    Receiver<T> makeReceiver(const std::function<void(const T& data)>& callback);  // lambda version

    template<typename T, typename ThreadObjectType>
    Receiver<T> makeReceiver(ThreadObjectType* object, void (ThreadObjectType::*callbackPtr)(const T& data));  // member function version

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
Transmitter<T> Thread::makeTransmitter()
{
    return std::make_shared<TransmitterImpl<T>>();
}

template<typename T>
Receiver<T> Thread::makeReceiver(const std::function<void(const T& data)>& callback)
{
    auto receiver = std::make_shared<ReceiverImpl<T>>(callback);
    mReceivers.push_back(receiver);
    return receiver;
}

template<typename T, typename ThreadObjectType>
Receiver<T> Thread::makeReceiver(ThreadObjectType *object, void (ThreadObjectType::*callbackPtr)(const T & data))
{
    auto receiver = std::make_shared<ReceiverImpl<T>>(object, callbackPtr);
    mReceivers.push_back(receiver);
    return receiver;
}


}

#endif
