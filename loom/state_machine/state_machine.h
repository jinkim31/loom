#ifndef LOOM_STATE_MACHINE_H
#define LOOM_STATE_MACHINE_H

#define LOOM_ENUM_STRING_PAIR(x) {x, #x}

#include "../threading/thread.h"
#include <unordered_map>
#include <iostream>
#include <functional>
#include <optional>

namespace loom
{

template <typename StateType, typename EventType>
struct Transition
{
    StateType startState;
    EventType event;

    bool operator==(const Transition &other) const
    {
        return (startState == other.startState && event == other.event);
    }
};
}

namespace std
{
template <typename StateType, typename EventType>
struct hash<loom::Transition<StateType, EventType>>
{
    size_t operator()(const loom::Transition<StateType, EventType> &key) const
    {
        return key.startState + key.event * 999;
    }
};
}

namespace loom
{
template <typename StateType, typename EventType>
class StateMachine
{
public:
    StateMachine(
            Thread& thread,
            typename Server<EventType, std::pair<StateType, StateType>>::SharedPtr& transitionServer,
            const std::string& name = "stateMachine")
    {
        mName = name;
        mVerbose = false;
        mStateChangedCallback = nullptr;

        transitionServer = thread.makeServer<EventType, std::pair<StateType, StateType>>([&](const EventType& event){
            auto oldState = currentState();
            notifyEvent(event);
            return std::pair(oldState, currentState());
        });
    }

    void setVerbose(bool verbose)
    {
        mVerbose = verbose;
    }
    void setState(const StateType& state){mCurrentState = state;}

    void setStateNameMap(const std::unordered_map<StateType, std::string>& stateNameMap){mStateNameMap = stateNameMap;}
    void setEventNameMap(const std::unordered_map<EventType, std::string>& eventNameMap){mEventNameMap = eventNameMap;}

    StateMachine& addTransition(StateType sourceState, EventType event, StateType destinationState, const std::function<void(void)>& handler=NULL)
    {
        mTransitions.insert({{sourceState, event}, {destinationState, handler}});
        mChainingState = destinationState;
        return *this;
    }

    StateMachine& chainTransition(EventType event, StateType destinationState, const std::function<void(void)>& handler=NULL)
    {
        mTransitions.insert({{mChainingState, event}, {destinationState, handler}});
        mChainingState = destinationState;
        return *this;
    }

    StateType currentState()
    {
        return mCurrentState;
    }

    void notifyEvent(EventType event)
    {
        Transition<StateType, EventType> t{};
        t.startState = mCurrentState;
        t.event = event;
        const auto &iter = mTransitions.find(t);

        // destination state not found
        if (iter == mTransitions.end())
        {
            if(mVerbose)
                std::cerr<<mName<<": "
                         <<getStateNameFromMap(mCurrentState)
                         <<" > "
                         <<getEventNameFromMap(event)
                         <<" > [NONE]"<<std::endl;
            return;
        }

        // found
        if(mVerbose)
            std::cout<<mName<<": "
                     <<getStateNameFromMap(mCurrentState)
                     <<" > "
                     <<getEventNameFromMap(event)
                     <<" > "
                     <<getStateNameFromMap(iter->second.first)
                     <<std::endl;

        const auto& handler = iter->second.second;
        mCurrentState = iter->second.first;

        if(handler)
            handler();

        if(mStateChangedCallback)
            mStateChangedCallback();

        if(mReservedEvent.has_value())
        {
            EventType reservedEvent = mReservedEvent.value();
            mReservedEvent = std::nullopt;
            notifyEvent(reservedEvent);
        }
    }

    void reserveEvent(EventType event)
    {
        mReservedEvent = event;
    }

    void setStateChangedCallback(const std::function<void()>& functor)
    {
        mStateChangedCallback = functor;
    }

    std::string getStateNameFromMap(const StateType& index)
    {
        auto itr = mStateNameMap.find(index);
        if(itr == mStateNameMap.end())
            return "[" + std::to_string(index) + "]";
        return itr->second;
    }

    std::string getEventNameFromMap(const EventType& index)
    {
        auto itr = mEventNameMap.find(index);
        if(itr == mEventNameMap.end())
            return "[" + std::to_string(index) + "]";
        return itr->second;
    }

private:
    StateType mCurrentState;
    StateType mChainingState;
    bool mVerbose;
    std::string mName;
    std::optional<EventType> mReservedEvent;
    std::function<void()> mStateChangedCallback;
    std::unordered_map<StateType, std::string> mStateNameMap;
    std::unordered_map<EventType, std::string> mEventNameMap;
    std::unordered_map<Transition<StateType, EventType>, std::pair<StateType, std::function<void(void)>>> mTransitions;
};
}

#endif
