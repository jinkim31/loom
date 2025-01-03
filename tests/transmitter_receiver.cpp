#include <iostream>
#include "loom/loom.h"


class TransmitterThread : public loom::Thread
{
public:
    TransmitterThread()
    {
        transmitter = makeTransmitter<int>();
    }

    loom::Transmitter<int>::SharedPtr transmitter;

protected:
    void step() override
    {
        std::cout << "transmitter transmitted : " << mCount << std::endl;
        transmitter->transmit(mCount++);
    }

private:
    int mCount = 0;
};

class ReceiverThread : public loom::Thread
{
public:
    ReceiverThread()
    {
        receiver = makeReceiver(&ReceiverThread::callback);
    }

    loom::Receiver<int>::SharedPtr receiver;

private:
    void callback(const int& data)
    {
        std::cout << "receiver received: " << data << std::endl;
    }
};

int main()
{
    auto transmitterThread = TransmitterThread();
    auto receiverThread = ReceiverThread();
    transmitterThread.transmitter->link(receiverThread.receiver);
    transmitterThread.start();
    receiverThread.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    transmitterThread.stop();
    receiverThread.stop();
    transmitterThread.transmitter->unlink(receiverThread.receiver);
}
