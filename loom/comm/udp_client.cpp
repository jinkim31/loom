#include "udp_client.h"
#include <Poco/Net/NetException.h>


inkjet::UDPClient::UDPClient()
{

}

bool inkjet::UDPClient::open(const std::string &ipString, const unsigned short& port)
{
    Poco::Net::SocketAddress address(Poco::Net::IPAddress(ipString), port);
    mSocket.connect(address);
    return true;
}

bool inkjet::UDPClient::close()
{
    mSocket.close();
    return true;
}

void inkjet::UDPClient::write(const std::string &data)
{
    try{
        size_t nSent = mSocket.sendBytes(data.data(), data.length());
        if(nSent != data.length())
            std::cerr<<"could not write all"<<std::endl;
    }catch(Poco::Net::NetException e)
    {
        std::cerr<<"UDPClient write() failed: "<<e.what()<<std::endl;
    }
}

void inkjet::UDPClient::write(const std::vector<uint8_t> &data)
{
    try{
        mSocket.sendBytes(data.data(), data.size());
    }catch(Poco::Net::NetException e)
    {
        std::cerr<<"UDPClient write() failed";
    }
}

std::vector<uint8_t> inkjet::UDPClient::read()
{
    poll();
    auto readBufferCopied =  mReadBuffer;
    mReadBuffer.clear();
    return readBufferCopied;
}

size_t inkjet::UDPClient::readMem(uint8_t* buffer, const int& bufferSize)
{
    if (mSocket.available())
    {
        size_t n = mSocket.receiveBytes(buffer, bufferSize);
        return n;
    }
    return 0;
}

size_t inkjet::UDPClient::poll()
{
    try
    {
        const size_t READ_BUFFER_SIZE = 1024;
        static uint8_t readBuffer[READ_BUFFER_SIZE];
        size_t nRead = 0;
        if (mSocket.available())
            nRead = mSocket.receiveBytes(readBuffer, READ_BUFFER_SIZE);
        for(int i=0; i<nRead; i++)
            mReadBuffer.push_back(readBuffer[i]);
        return nRead;
    }catch(Poco::Net::NetException e)
    {
        std::cerr<<"UDPClient poll() failed";
        return 0;
    }
}

size_t inkjet::UDPClient::available()
{
    poll();
    return mReadBuffer.size();
}