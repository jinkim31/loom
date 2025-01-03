#include <iostream>
#include "loom/thread.h"
#include "loom/server_client.h"

class ProducerThread : public loom::Thread
{
public:
    ProducerThread() : Thread("Server", 100)
    {
        /* lambda version
        server = makeServer<int, double>([](const int& arg){
            std::cout<<"server received arg "<<arg<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            return 3.14 * arg;
        });
         */
        server = makeServer<int, double>(this, &ProducerThread::callback);
    }
    loom::Server<int, double>::SharedPtr server;
private:
    double callback(const int& arg)
    {
        return 3.14 * arg;
    }
};

class ConsumerThread : public loom::Thread
{
public:
    ConsumerThread() : Thread("Client", 100)
    {
        /* lambda version
        client = makeClient<int, double>([](const double& ret){
            std::cout<<"client received ret: "<<ret<<std::endl;
        });
         */
        client = makeClient<int, double>(this, &ConsumerThread::callback);
    }
    loom::Client<int, double>::SharedPtr client;
protected:
    void step() override
    {
        client->requestAsync(count++);
    }
private:
    void callback(const double& ret)
    {
        std::cout<<"client received ret: "<<ret<<std::endl;
    }
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
