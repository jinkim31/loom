#ifndef LOOM_THREAD_H
#define LOOM_THREAD_H

#include <iostream>
#include <thread>
#include <functional>
#include "transmitter_receiver.h"
#include "server_client.h"


namespace loom
{

class Thread
{
public:
    Thread();
    virtual ~Thread()= default;

protected:
    virtual void loopCallback(){}

    template<typename T>
    typename Transmitter<T>::SharedPtr makeTransmitter();

    template<typename T>
    typename Receiver<T>::SharedPtr makeReceiver(const std::function<void(const T& data)>& callback);

    template<typename T, typename ThreadObjectType>
    typename Receiver<T>::SharedPtr makeReceiver(void (ThreadObjectType::*callbackPtr)(const T &));

    template<typename ArgType, typename RetType>
    typename Server<ArgType, RetType>::SharedPtr makeServer(const std::function<RetType(const ArgType&)>& callback);

    template<typename ArgType, typename RetType, typename ThreadObjectType>
    typename Server<ArgType, RetType>::SharedPtr makeServer(RetType (ThreadObjectType::*callbackPtr)(const ArgType&));

    template<typename ArgType, typename RetType>
    typename Client<ArgType, RetType>::SharedPtr makeClient(const std::function<void (const RetType&)>& callback=[](const RetType&){});

    template<typename ArgType, typename RetType, typename ThreadObjectType>
    typename Client<ArgType, RetType>::SharedPtr makeClient(void (ThreadObjectType::*callbackPtr)(const RetType&));

    std::vector<std::shared_ptr<ReceiverInterface>> mReceivers;
    std::string mName;
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
typename Receiver<T>::SharedPtr Thread::makeReceiver(void (ThreadObjectType::*callbackPtr)(const T &))
{
    auto receiver = std::make_shared<Receiver<T>>([=](const T& data){((ThreadObjectType*)this->*callbackPtr)(data);});
    mReceivers.push_back(receiver);
    return receiver;
}

template<typename ArgType, typename RetType>
typename Server<ArgType, RetType>::SharedPtr loom::Thread::makeServer(const std::function<RetType(const ArgType &)> &callback)
{
    auto transmitter = makeTransmitter<RetType>();
    auto receiver = makeReceiver<ArgType>([=](const ArgType& arg){
        transmitter->transmit(callback(arg));
    });
    return std::make_shared<Server<ArgType, RetType>>(
            std::move(receiver),
            std::move(transmitter));
}

template<typename ArgType, typename RetType, typename ThreadObjectType>
typename Server<ArgType, RetType>::SharedPtr loom::Thread::makeServer(RetType (ThreadObjectType::*callbackPtr)(const ArgType&))
{
    auto transmitter = makeTransmitter<RetType>();
    auto receiver = makeReceiver<ArgType>([=](const ArgType& arg){
        transmitter->transmit(((ThreadObjectType*)this->*callbackPtr)(arg));
    });
    return std::make_shared<Server<ArgType, RetType>>(
            std::move(receiver),
            std::move(transmitter));
}

template<typename ArgType, typename RetType>
typename Client<ArgType, RetType>::SharedPtr loom::Thread::makeClient(const std::function<void(const RetType &)> &callback)
{
    auto transmitter = makeTransmitter<ArgType>();
    auto receiver = makeReceiver<RetType>(callback);
    return std::make_shared<Client<ArgType, RetType>>(
            std::move(transmitter),
            std::move(receiver));
}

template<typename ArgType, typename RetType, typename ThreadObjectType>
typename Client<ArgType, RetType>::SharedPtr loom::Thread::makeClient(void (ThreadObjectType::*callbackPtr)(const RetType&))
{
    auto transmitter = makeTransmitter<ArgType>();
    auto receiver = makeReceiver<RetType>([=](const RetType& arg){((ThreadObjectType*)this->*callbackPtr)(arg);});
    return std::make_shared<Client<ArgType, RetType>>(
            std::move(transmitter),
                    std::move(receiver));
}


class LoopingThread : public Thread
{
public:
    LoopingThread(std::chrono::high_resolution_clock::duration loopInterval=std::chrono::milliseconds(10));
    virtual ~LoopingThread()=default;
    void start();
    void stop();
private:
    void runLoop();
    static void* entryPoint(void *param);
    std::chrono::high_resolution_clock::duration mLoopInterval;
    std::atomic<bool> mIsThreadRunning, mEventLoopBreakFlag;
    std::thread mThread;
};

class ManualThread : public Thread
{
public:
    ManualThread(std::chrono::high_resolution_clock::duration loopInterval=std::chrono::milliseconds(10));
    void step();
private:
    std::chrono::high_resolution_clock::duration mLoopInterval;
};

}

#endif
