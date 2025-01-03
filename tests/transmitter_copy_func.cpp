#include "loom/loom.h"

class CopyProbe {
public:
    explicit CopyProbe(bool printMessage=true)
    {
        mDataPtr = std::make_shared<int>();
        if(printMessage)
            std::cout << toString() << " constructed." << std::endl;
    }

    CopyProbe(const CopyProbe& other) : mDataPtr(other.mDataPtr)
    {
        std::cout << toString() << " copy constructed." << std::endl;
    }

    CopyProbe(CopyProbe&& other) noexcept : mDataPtr(other.mDataPtr)
    {
        std::cout << toString() << " move constructed." << std::endl;
    }

    ~CopyProbe()
    {
        //std::cout << toString() << " destructed." << std::endl;
    }

    CopyProbe clone() const
    {
        CopyProbe copyProbe(false);
        copyProbe.mDataPtr = std::make_shared<int>(*mDataPtr);
        std::cout << copyProbe.toString() << " cloned constructed." << std::endl;
        return copyProbe;
    }

    std::string toString() const
    {
        return "CopyProbe with data pointer " + std::to_string((long long)mDataPtr.get());
    }

private:
    std::shared_ptr<int> mDataPtr;
};

class MainThread : public loom::ManualThread
{
public:
    MainThread()
    {
        copyTransmitter = makeTransmitter<CopyProbe>();
        cloneTransmitter = makeTransmitter<CopyProbe>([](const CopyProbe& copyProbe){return copyProbe.clone();});
    }
    loom::Transmitter<CopyProbe>::SharedPtr copyTransmitter;
    loom::Transmitter<CopyProbe>::SharedPtr cloneTransmitter;
};

class ConsumerThread : public loom::ManualThread
{
public:
    ConsumerThread()
    {
        receiver = makeReceiver<CopyProbe>(&ConsumerThread::receiverCallback);
    }
    loom::Receiver<CopyProbe>::SharedPtr receiver;
private:
    void receiverCallback(const CopyProbe& copyProbe)
    {
        std::cout<<">> Consumer thread received "<<copyProbe.toString()<<std::endl;
    }
};


int main()
{
    MainThread mainThread{};
    ConsumerThread consumerThread1, consumerThread2;
    mainThread.copyTransmitter->link(consumerThread1.receiver);
    mainThread.copyTransmitter->link(consumerThread2.receiver);
    mainThread.cloneTransmitter->link(consumerThread1.receiver);
    mainThread.cloneTransmitter->link(consumerThread2.receiver);

    std::cout<<"========[ Copy transmit]========"<<std::endl;
    mainThread.copyTransmitter->transmit(CopyProbe());
    std::cout<<"========[ Copy consumer 1 receive ]========"<<std::endl;
    consumerThread1.step();
    std::cout<<"========[ Copy consumer 2 receive ]========"<<std::endl;
    consumerThread2.step();

    std::cout<<"========[ Clone transmit]========"<<std::endl;
    mainThread.cloneTransmitter->transmit(CopyProbe());
    std::cout<<"========[ Clone consumer 1 receive ]========"<<std::endl;
    consumerThread1.step();
    std::cout<<"========[ Clone consumer 2 receive ]========"<<std::endl;
    consumerThread2.step();

    mainThread.copyTransmitter->unlink(consumerThread1.receiver);
    mainThread.copyTransmitter->unlink(consumerThread2.receiver);
    mainThread.cloneTransmitter->unlink(consumerThread1.receiver);
    mainThread.cloneTransmitter->unlink(consumerThread2.receiver);
}