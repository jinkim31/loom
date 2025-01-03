#include <iostream>
#include "loom/thread.h"
#include "loom/server_client.h"

class ServerThread : public loom::Thread
{
public:
    ServerThread()
    {
        server = makeServer<int, double>(this, &ServerThread::callback);
    }

    loom::Server<int, double>::SharedPtr server;

private:
    double callback(const int& arg)
    {
        std::cout<<"server received arg "<<arg<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return 3.14 * arg;
    }
};

class ClientThread : public loom::Thread
{
public:
    ClientThread()
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
    auto serverThread = ServerThread();
    auto clientThread = ClientThread();
    serverThread.server->link(clientThread.client);
    serverThread.start();
    clientThread.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    serverThread.stop();
    clientThread.stop();
    serverThread.server->unlink(clientThread.client);
}
