#include <iostream>
#include "loom/loom/loom.h"


class TransmitterThread : public loom::LoopingThread
{
public:
    TransmitterThread()
    {
        transmitter = makeTransmitter<int>();
    }

    loom::Transmitter<int>::SharedPtr transmitter;

protected:
    void loopCallback() override
    {
        transmitter->transmit(mCount);
        std::cout << "Transmitter thread transmitted "<<mCount<<"."<<std::endl;
        mCount++;
    }

private:
    int mCount = 0;
};

class ReceiverThread : public loom::LoopingThread
{
public:
    ReceiverThread()
    {
        receiver = makeReceiver(&ReceiverThread::receiveCallback);
    }

    loom::Receiver<int>::SharedPtr receiver;

private:
    void receiveCallback(const int& data)
    {
        std::cout<<"Receiver thread received "<<data<<"."<<std::endl;
    }
};

int main()
{
    TransmitterThread transmitterThread;
    ReceiverThread receiverThread;
    transmitterThread.transmitter->link(receiverThread.receiver);
    transmitterThread.start();
    receiverThread.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    transmitterThread.stop();
    receiverThread.stop();
    transmitterThread.transmitter->unlink(receiverThread.receiver);
}
