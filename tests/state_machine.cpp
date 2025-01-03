#include "loom/loom.h"


enum State : int
{
    STATE_IDLE,
    STATE_RUN,
};

enum Event : int
{
    EVENT_START,
    EVENT_STOP,
};

std::unordered_map<State, std::string> stateNameMap{
    LOOM_ENUM_STRING_PAIR(STATE_IDLE),
    LOOM_ENUM_STRING_PAIR(STATE_RUN),
};

std::unordered_map<Event, std::string> eventNameMap{
    LOOM_ENUM_STRING_PAIR(EVENT_START),
    LOOM_ENUM_STRING_PAIR(EVENT_STOP),
};

class MainThread : public loom::LoopingThread
{
public:
    MainThread() : stateMachine(*this, transitionServer)
    {
        // setup state machine
        stateMachine.setStateNameMap(stateNameMap);
        stateMachine.setEventNameMap(eventNameMap);
        stateMachine.setState(STATE_IDLE);
        stateMachine.addTransition(STATE_IDLE, EVENT_START, STATE_RUN)
                .chainTransition(EVENT_STOP, STATE_IDLE);
        stateMachine.setVerbose(false);
    }

    loom::Server<Event, std::pair<State, State>>::SharedPtr transitionServer;
private:
    loom::StateMachine<State, Event> stateMachine;
};

class TransitionThread : public loom::LoopingThread
{
public:
    TransitionThread(Event event, std::string name) : loom::LoopingThread(std::chrono::milliseconds(100), name)
    {
        mEvent = event;
        transitionClient = makeClient<Event, std::pair<State, State>>();
    }
    loom::Client<Event, std::pair<State, State>>::SharedPtr transitionClient;
protected:
    void loopCallback() override
    {
        auto states = transitionClient->requestSync(mEvent);
        if(states.has_value())
            std::cout<<name<<" changed state from "
            <<stateNameMap[states.value().first]<<" to "
            <<stateNameMap[states.value().second]<<"."<<std::endl;
    }

private:
    Event mEvent;

};

int main()
{
    MainThread mainThread;
    TransitionThread transitionThread1(EVENT_START, "transitionThread1"), transitionThread2(EVENT_STOP, "transitionThread2");
    mainThread.transitionServer->link(transitionThread1.transitionClient);
    mainThread.transitionServer->link(transitionThread2.transitionClient);

    mainThread.start();
    transitionThread1.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    transitionThread2.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    mainThread.stop();
    transitionThread1.stop();
    transitionThread2.stop();

    mainThread.transitionServer->unlink(transitionThread1.transitionClient);
    mainThread.transitionServer->unlink(transitionThread2.transitionClient);
}