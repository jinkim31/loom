#ifndef LOOM_THREADING_TRANSMITTER_RECEIVER_H
#define LOOM_THREADING_TRANSMITTER_RECEIVER_H

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
class LoopingThread;
class ManualThread;

// receiver
class ReceiverInterface
{
protected:
    virtual void receiveCallback()=0;
    virtual size_t size()=0;
    friend class Thread;
    friend class LoopingThread;
    friend class ManualThread;
};

template<typename T>
class Receiver : public ReceiverInterface
{
public:
    Receiver(const std::function<void(const T &)> &callback);
    ~Receiver();
    size_t size() override;
    void clear();
    T receive();
    using SharedPtr = std::shared_ptr<Receiver<T>>;
private:
    void receiveCallback() override;
    void notifyLink(Transmitter<T>* transmitter); // should be called in the main thread
    void notifyUnlink(Transmitter<T>* transmitter); // should be called in the main thread
    Queue<T> mQueue;
    std::set<Transmitter<T>*> mLinkedTransmitters;
    std::function<void(const T &)> mCallback;
    friend class Transmitter<T>;
protected:
};

template<typename T>
Receiver<T>::Receiver(const std::function<void(const T &)> &callback)
{
    mCallback = callback;
}

template<typename T>
Receiver<T>::~Receiver()
{
    if(!mLinkedTransmitters.empty())
        std::cerr<<"[ Loom ] Receiver remained linked to Transmitter(s) when it is destructed."
                   " Ensure to unlink using Transmitter->unlink() before destruction."<<std::endl;
}

template<typename T>
T Receiver<T>::receive()
{
    return mQueue.pop();
}

template<typename T>
size_t Receiver<T>::size()
{
    return mQueue.size();
}

template<typename T>
void Receiver<T>::clear()
{
    mQueue.clear();
}

template<typename T>
void Receiver<T>::notifyLink(Transmitter<T> *transmitter)
{
    mLinkedTransmitters.insert(transmitter);
}

template<typename T>
void Receiver<T>::notifyUnlink(Transmitter<T> *transmitter)
{
    mLinkedTransmitters.erase(transmitter);
}



template<typename T>
void Receiver<T>::receiveCallback()
{
    mCallback(mQueue.pop());
}


// transmitter
template<typename T>
class Transmitter
{
public:
    Transmitter(const std::function<T(const T&)>& cloneFunctor);
    ~Transmitter();
    void link(std::shared_ptr<Receiver<T>> receiver);
    void unlink(std::shared_ptr<Receiver<T>> receiver);
    void transmit(const T &data);
    using SharedPtr = std::shared_ptr<Transmitter<T>>;

private:
    std::mutex mMutex;
    std::set<Receiver<T>*> mReceivers;
    std::function<T(const T &)> mCopyFunc;
};

template<typename T>
Transmitter<T>::Transmitter(const std::function<T(const T &)> &cloneFunctor)
{
    mCopyFunc = cloneFunctor;
}

template<typename T>
Transmitter<T>::~Transmitter()
{
    if(!mReceivers.empty())
        std::cerr<<"[ Loom ] Transmitter remained linked to Receiver(s) when it is destructed."
                   " Ensure to unlink using Transmitter->unlink() before destruction."<<std::endl;
}

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
        receiver->mQueue.push(std::move(mCopyFunc(data)));
    }
}

}

#endif
