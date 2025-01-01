#include <iostream>
#include "thread.h"

class ProducerThread : public loom::Thread
{
public:
    ProducerThread()
    {

    }
    loom::Transmitter<int> transmitter;
protected:
    void step() override
    {
        std::cout<<"produce"<<std::endl;
    }
};

class ConsumerThread : public loom::Thread
{
public:
    ConsumerThread()
    {

    }
    loom::Receiver<int> receiver;
protected:
    void step() override
    {
        std::cout<<"consume"<<std::endl;
    }
};

int main()
{
    auto producer = ProducerThread();
    auto consumer = ConsumerThread();
    producer.start();
    consumer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    producer.stop();
    consumer.stop();
}
