#include "loom/loom/loom.h"


class MainThread : public loom::ManualThread
{
public:
    MainThread()
    {
        receiver = makeReceiver<int>(&MainThread::receiveCallback);
    }

    loom::Receiver<int>::SharedPtr receiver;

private:
   void receiveCallback(const int& data)
    {
       std::cout<<"Main thread received "<<data<<"."<<std::endl;
    }
};


class ProducerThread : public loom::LoopingThread
{
public:
    ProducerThread()
    {
        transmitter = makeTransmitter<int>();
    }

    loom::Transmitter<int>::SharedPtr transmitter;

protected:
    void loopCallback() override
    {
        transmitter->transmit(count);
        std::cout<<"Producer thread transmitted "<<count<<"."<<std::endl;
        count++;
    }

private:
    int count = 0;
};

int main()
{
    MainThread mainThread;
    ProducerThread producerThread;

    producerThread.transmitter->link(mainThread.receiver);
    producerThread.start();

    auto start = std::chrono::high_resolution_clock::now();
    while (std::chrono::high_resolution_clock::now() - start <  std::chrono::seconds(3))
    {
        mainThread.step();
    }

    producerThread.stop();
    producerThread.transmitter->unlink(mainThread.receiver);
}