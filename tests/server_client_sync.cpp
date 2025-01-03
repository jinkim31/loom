#include <iostream>
#include "loom/thread.h"
#include "loom/server_client.h"

class ProducerThread : public loom::Thread
{
public:
    ProducerThread() : Thread("Server", 10)
    {
        server = makeServer<int, double>([](const int& arg){
            std::cout<<"server received arg "<<arg<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            return 3.14 * arg;
        });
    }
    loom::Server<int, double>::SharedPtr server;
private:

};

class ConsumerThread : public loom::Thread
{
public:
    ConsumerThread() : Thread("Client", 10)
    {
        client = makeClient<int, double>();
    }
    loom::Client<int, double>::SharedPtr client;
protected:
    void step() override
    {
        std::cout<<client->requestSync(count++)<<std::endl;
    }
private:
    int count = 0;
};

int main()
{
    auto serverThread = ProducerThread();
    auto clientThread = ConsumerThread();
    serverThread.server->link(clientThread.client);
    serverThread.start();
    clientThread.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    serverThread.stop();
    clientThread.stop();
    serverThread.server->unlink(clientThread.client);
}
