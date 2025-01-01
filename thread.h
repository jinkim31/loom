#ifndef LOOM_THREAD_H
#define LOOM_THREAD_H

#include "inter_thread_communication.h"
#include <iostream>
#include <thread>

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
    Receiver <T> makeReceiver();

    virtual void step(){}

private:
    static void* entryPoint(void *param);
    void runEventLoop();

    std::chrono::high_resolution_clock::duration mEventLoopDelay;
    std::mutex mMutex;
    bool mIsThreadRunning, mEventLoopBreakFlag;
    std::thread mThread;
};

template<typename T>
Transmitter<T> Thread::makeTransmitter()
{
    return Transmitter<T>();
}

template<typename T>
Receiver<T> Thread::makeReceiver()
{
    return Receiver<T>();
}

}

#endif
