#ifndef LOOM_SERVICE_H
#define LOOM_SERVICE_H

#include "itc.h"

namespace loom
{

template<typename ArgType, typename RetType>
class Server
{
public:
    Server(const std::function<RetType(const ArgType& arg)>& callback)
    {
        mCallback = callback;
    }

    typename Receiver<ArgType>::SharedPtr receiver;
    typename Transmitter<RetType>::SharedPtr transmitter;
    using SharedPtr = std::shared_ptr<Server<ArgType, RetType>>;
private:
    std::function<RetType(const ArgType& arg)>& mCallback;
};

template<typename ArgType, typename RetType>
class Client
{
public:
    Client()
    {

    }
    typename Transmitter<ArgType>::SharedPtr transmitter;
    typename Receiver<RetType>::SharedPtr receiver;
    using SharedPtr = std::shared_ptr<Client<ArgType, RetType>>;
};

}

#endif
