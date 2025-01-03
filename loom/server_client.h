#ifndef LOOM_SERVER_CLIENT_H
#define LOOM_SERVER_CLIENT_H

#include "transmitter_receiver.h"

namespace loom
{

// forward declaration
template<typename ArgType, typename RetType>
class Client;


template<typename ArgType, typename RetType>
class Server
{
public:
    Server(
            const typename Receiver<ArgType>::SharedPtr && receiver,
            const typename Transmitter<RetType>::SharedPtr && transmitter)
    {

        mReceiver = std::move(receiver);
        mTransmitter = std::move(transmitter);
    }

    void link(typename Client<ArgType, RetType>::SharedPtr& client);
    void unlink(typename Client<ArgType, RetType>::SharedPtr& client);
    using SharedPtr = std::shared_ptr<Server<ArgType, RetType>>;
private:
    typename Receiver<ArgType>::SharedPtr mReceiver;
    typename Transmitter<RetType>::SharedPtr mTransmitter;
};

template<typename ArgType, typename RetType>
void Server<ArgType, RetType>::link(typename Client<ArgType, RetType>::SharedPtr& client)
{
    client->mTransmitter->link(mReceiver);
    mTransmitter->link(client->mReceiver);
}

template<typename ArgType, typename RetType>
void Server<ArgType, RetType>::unlink(typename Client<ArgType, RetType>::SharedPtr& client)
{
    client->mTransmitter->unlink(mReceiver);
    mTransmitter->unlink(client->mReceiver);
}

template<typename ArgType, typename RetType>
class Client
{
public:
    Client(
            const typename Transmitter<ArgType>::SharedPtr && transmitter,
            const typename Receiver<RetType>::SharedPtr && receiver)
    {
        mTransmitter = std::move(transmitter);
        mReceiver = std::move(receiver);
    }
    using SharedPtr = std::shared_ptr<Client<ArgType, RetType>>;
    friend class Server<ArgType, RetType>;

    void requestAsync(const ArgType& arg);
    void requestSync(const ArgType& arg);
private:
    typename Transmitter<ArgType>::SharedPtr mTransmitter;
    typename Receiver<RetType>::SharedPtr mReceiver;
};

template<typename ArgType, typename RetType>
void Client<ArgType, RetType>::requestSync(const ArgType &arg)
{

}

template<typename ArgType, typename RetType>
void Client<ArgType, RetType>::requestAsync(const ArgType &arg)
{
    mTransmitter->transmit(arg);
}

}

#endif
