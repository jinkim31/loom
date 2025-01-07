#include "thread.h"

loom::Thread::Thread(const std::string& name) : name(name)
{

}

loom::LoopingThread::LoopingThread(
        const std::chrono::high_resolution_clock::duration &loopInterval,
        const std::string &name) : Thread(name)
{
    mIsThreadRunning = false;
    mLoopInterval = loopInterval;
}

void loom::LoopingThread::start()
{
    if(mIsThreadRunning)
        return;
    mThread = std::thread(LoopingThread::entryPoint, this);
    mIsThreadRunning = true;
    mEventLoopBreakFlag = false;
}

void loom::LoopingThread::stop()
{
    if(!mIsThreadRunning)
        return;
    mEventLoopBreakFlag = true;
    if(mThread.joinable())
        mThread.join();
    mIsThreadRunning = false;
}

void *loom::LoopingThread::entryPoint(void *param)
{
    auto* ethreadPtr = (LoopingThread*)param;
    ethreadPtr->runLoop();
    return nullptr;
}

void loom::LoopingThread::runLoop()
{
    while(true)
    {
        // check thread loop break
        if (mEventLoopBreakFlag)
            return;

        // receiver callbacks
        for(const auto& receiver : mReceivers)
        {
            for(int i=0; i< receiver->size(); i++)
            {
                receiver->receiveCallback();
                if (mEventLoopBreakFlag)
                    return;
            }
        }

        loopCallback();
        std::this_thread::sleep_for(mLoopInterval);
    }
}

loom::ManualThread::ManualThread(std::chrono::high_resolution_clock::duration loopInterval)
{
    mLoopInterval = loopInterval;
}

void loom::ManualThread::step()
{
    // receiver callbacks
    for(const auto& receiver : mReceivers)
    {
        for(int i=0; i< receiver->size(); i++)
        {
            receiver->receiveCallback();
        }
    }

    loopCallback();
    std::this_thread::sleep_for(mLoopInterval);
}

loom::Empty loom::Empty::empty;