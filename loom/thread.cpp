#include "thread.h"

loom::Thread::Thread(const std::string &name, const int &loopIntervalMilliseconds)
{
    mIsThreadRunning = false;
    mEventLoopDelay = std::chrono::milliseconds(loopIntervalMilliseconds);
    mName = name;
}

void loom::Thread::start()
{
    std::unique_lock<std::mutex> lock(mMutex);
    if(mIsThreadRunning)
        return;
    mThread = std::thread(Thread::entryPoint, this);
    mIsThreadRunning = true;
    mEventLoopBreakFlag = false;
}

void loom::Thread::stop()
{
    std::unique_lock<std::mutex> lock(mMutex);
    if(!mIsThreadRunning)
        return;
    mEventLoopBreakFlag = true;
    if(mThread.joinable())
        mThread.join();
    mIsThreadRunning = false;
}

void *loom::Thread::entryPoint(void *param)
{
    auto* ethreadPtr = (Thread*)param;
    ethreadPtr->runEventLoop();
    return nullptr;
}

void loom::Thread::runEventLoop()
{
    while(true)
    {
        // check thread loop break
        if (mEventLoopBreakFlag)
            return;

        // receiver callbacks
        for(const auto& receiver : mReceivers)
        {
            for(int i=0; i<receiver->nAvailable(); i++)
            {
                receiver->receiveCallback();
                if (mEventLoopBreakFlag)
                    return;
            }
        }


        step();
        std::this_thread::sleep_for(mEventLoopDelay);
    }
}
