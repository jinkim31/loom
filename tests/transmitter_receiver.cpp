#include <iostream>
#include "loom/thread.h"

class ServerThread : public loom::Thread
{
public:
    ServerThread() : Thread(<#initializer#>, <#initializer#>), Thread(<#initializer#>, <#initializer#>)
    {
        transmitter = makeTransmitter<int>();
    }
    loom::Transmitter<int>::SharedPtr transmitter;
protected:
    void step() override
    {
        std::cout<<"produce: "<<mCount<<std::endl;
        transmitter->transmit(mCount++);
    }
private:
    int mCount = 0;
};

class ClientThread : public loom::Thread
{
public:
    ClientThread() : Thread(<#initializer#>, <#initializer#>), Thread(<#initializer#>, <#initializer#>)
    {
        //receiver = makeReceiver<int>([](const int& data){std::cout<<"received data "<<data<<std::endl;});
        receiver = makeReceiver<int>(this, &ClientThread::callback);
    }
    loom::Receiver<int>::SharedPtr receiver;
private:
    void callback(const int& data)
    {
        std::cout<<"consume: "<<data<<std::endl;
    }
};

int main()
{
    auto producer = ServerThread();
    auto consumer = ClientThread();
    producer.transmitter->link(consumer.receiver);
    producer.start();
    consumer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    producer.stop();
    consumer.stop();
    producer.transmitter->unlink(consumer.receiver);
}
