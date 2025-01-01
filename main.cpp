#include <iostream>
#include "thread.h"

class ProducerThread : public loom::Thread
{
public:
    ProducerThread()
    {
        transmitter = makeTransmitter<int>();
    }
    loom::Transmitter<int> transmitter;
protected:
    void step() override
    {
        std::cout<<"produce: "<<mCount<<std::endl;
        transmitter->transmit(mCount++);
    }
private:
    int mCount = 0;
};

class ConsumerThread : public loom::Thread
{
public:
    ConsumerThread()
    {
        //receiver = makeReceiver<int>([](const int& data){std::cout<<"received data "<<data<<std::endl;});
        receiver = makeReceiver<int>(this, &ConsumerThread::callback);
    }
    loom::Receiver<int> receiver;
private:
    void callback(const int& data)
    {
        std::cout<<"consume: "<<data<<std::endl;
    }
};

int main()
{
    auto producer = ProducerThread();
    auto consumer = ConsumerThread();
    producer.transmitter->link(consumer.receiver);
    producer.start();
    consumer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    producer.stop();
    consumer.stop();
    producer.transmitter->unlink(consumer.receiver);
}
