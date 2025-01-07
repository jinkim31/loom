#include <iostream>
#include "loom/loom/loom.h"


class ServerThread : public loom::LoopingThread
{
public:
    ServerThread()
    {
        server = makeServer<int, double>(&ServerThread::serverCallback);
    }

    loom::Server<int, double>::SharedPtr server;

private:
    double serverCallback(const int& arg)
    {
        std::cout <<"Server thread received arg "<<arg<<"."<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return 3.14 * arg;
    }
};

class ClientThread : public loom::LoopingThread
{
public:
    ClientThread()
    {
        client = makeClient<int, double>(&ClientThread::clientCallback);
    }

    loom::Client<int, double>::SharedPtr client;

protected:
    void loopCallback() override
    {
        client->requestAsync(count++);
    }

private:
    void clientCallback(const double& ret)
    {
        std::cout<<"Client thread received ret "<<ret<<"."<<std::endl;
    }

    int count = 0;
};

int main()
{
    ServerThread serverThread;
    ClientThread clientThread;
    serverThread.server->link(clientThread.client);
    serverThread.start();
    clientThread.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    serverThread.stop();
    clientThread.stop();
    serverThread.server->unlink(clientThread.client);
}
