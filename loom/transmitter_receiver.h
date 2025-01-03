#ifndef LOOM_TRANSMITTER_RECEIVER_H
#define LOOM_TRANSMITTER_RECEIVER_H

#include "queue.h"
#include <set>
#include <iostream>
#include <map>


namespace loom
{

// forward declaration
template<typename T>
class Transmitter;
class Thread;

// receiver
class ReceiverInterface
{
protected:
    virtual void receiveCallback()=0;
    virtual size_t nAvailable()=0;
    friend class Thread;
};

template<typename T>
class Receiver : public ReceiverInterface
{
public:
    Receiver(const std::function<void(const T &)> &callback);
    ~Receiver();
    size_t nAvailable() override;
    T receive();
    using SharedPtr = std::shared_ptr<Receiver<T>>;
private:
    void receiveCallback() override;
    void notifyLink(Transmitter<T>* transmitter);
    void notifyUnlink(Transmitter<T>* transmitter);
    Queue<T> mQueue;
    std::mutex mMutex;
    std::set<Transmitter<T>*> mLinkedTransmitters;
    std::function<void(const T &)> mCallback;
    friend class Transmitter<T>;
};

template<typename T>
T Receiver<T>::receive()
{
    return mQueue.pop();
}

template<typename T>
size_t Receiver<T>::nAvailable()
{
    return mQueue.len();
}

template<typename T>
Receiver<T>::~Receiver()
{
    if(!mLinkedTransmitters.empty())
        std::cerr<<"[ Loom ] Receiver remained linked to Transmitter(s) when it is destructed."
                   " Ensure to unlink using Transmitter->unlink() before destruction."<<std::endl;
}

template<typename T>
void Receiver<T>::notifyLink(Transmitter<T> *transmitter)
{

    std::unique_lock<std::mutex> lock(mMutex);
    mLinkedTransmitters.insert(transmitter);
}

template<typename T>
void Receiver<T>::notifyUnlink(Transmitter<T> *transmitter)
{
    std::unique_lock<std::mutex> lock(mMutex);
    mLinkedTransmitters.erase(transmitter);
}



template<typename T>
void Receiver<T>::receiveCallback()
{
    mCallback(mQueue.pop());
}

template<typename T>
Receiver<T>::Receiver(const std::function<void(const T &)> &callback)
{
    mCallback = callback;
}

// transmitter
template<typename T>
class Transmitter
{
public:
    Transmitter()= default;
    ~Transmitter();
    void link(std::shared_ptr<Receiver<T>> receiver);
    void unlink(std::shared_ptr<Receiver<T>> receiver);
    void transmit(const T &data);
    using SharedPtr = std::shared_ptr<Transmitter<T>>;

private:
    std::mutex mMutex;
    std::set<Receiver<T>*> mReceivers;
};

template<typename T>
void Transmitter<T>::link(std::shared_ptr<Receiver<T>> receiver)
{
    std::unique_lock<std::mutex> lock(mMutex);
    mReceivers.insert(receiver.get());
    receiver->notifyLink(this);
}

template<typename T>
void Transmitter<T>::unlink(std::shared_ptr<Receiver<T>> receiver)
{
    std::unique_lock<std::mutex> lock(mMutex);
    mReceivers.erase(receiver.get());
    receiver->notifyUnlink(this);
}

template<typename T>
void Transmitter<T>::transmit(const T &data)
{
    std::unique_lock<std::mutex> lock(mMutex);
    for (const auto& receiver : mReceivers)
    {
        receiver->mQueue.push(data);
    }
}

template<typename T>
Transmitter<T>::~Transmitter()
{
    if(!mReceivers.empty())
        std::cerr<<"[ Loom ] Transmitter remained linked to Receiver(s) when it is destructed."
                   " Ensure to unlink using Transmitter->unlink() before destruction."<<std::endl;
}

}

#endif
