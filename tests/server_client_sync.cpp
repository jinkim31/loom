#include <iostream>
#include "loom/loom.h"


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
        std::cout<<"Server received arg "<<arg<<"."<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return 3.14 * arg;
    }
};

class ClientThread : public loom::LoopingThread
{
public:
    ClientThread()
    {
        client = makeClient<int, double>();
    }

    loom::Client<int, double>::SharedPtr client;

protected:
    void loopCallback() override
    {
        double ret = client->requestSync(count++);
        std::cout << "Client received ret " << ret << "." << std::endl;
    }

private:
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
