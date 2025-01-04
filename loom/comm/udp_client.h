#ifndef LOOM_UDP_CLIENT_H
#define LOOM_UDP_CLIENT_H

#include <Poco/Net/Net.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Timespan.h>

namespace loom::comm
{

class UDPClient
{
public:
    UDPClient();
    bool open(const std::string& ipString, const unsigned short& port);
    bool close();
    void write(const std::string& data);
    void write(const std::vector<uint8_t>& data);
    std::vector<uint8_t> read();
    size_t readMem(uint8_t* buffer, const int& bufferSize);
    size_t available();
private:
    size_t poll();
    Poco::Net::DatagramSocket mSocket;
    std::vector<uint8_t> mReadBuffer;
};

}

#endif
