#include <loom/loom.h>
#include <cstdlib>
#include <ctime>

using namespace loom;

class ServerThread : public LoopingThread
{
public:
    ServerThread()
    {
        std::srand(std::time(nullptr));

        server = makeServer<Empty, int>([](const Empty&){
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return std::rand();
        });
    }
    Server<Empty, int>::SharedPtr server;
};

class Application : public ManualThread
{
public:
    Application()
    {
        // threading
        mClient = makeClient<Empty, int>([&](const int& number){
            std::cout<<"callback"<<std::endl;
            mNumber = number;
        });
        mServerThread.start();
        mServerThread.server->link(mClient);

        HelloImGui::RunnerParams runnerParams;
        runnerParams.callbacks.SetupImGuiStyle = inkjet::setStyle;
        runnerParams.callbacks.ShowGui = [&]{
            step();
            render();
        };
        runnerParams.callbacks.BeforeExit = [&]{
            mServerThread.stop();
            mServerThread.server->unlink(mClient);
        };
        runnerParams.imGuiWindowParams.defaultImGuiWindowType = HelloImGui::DefaultImGuiWindowType::NoDefaultWindow;
        runnerParams.callbacks.LoadAdditionalFonts = inkjet::initFont;
        runnerParams.imGuiWindowParams.enableViewports = false;
        runnerParams.fpsIdling.enableIdling = false;
        runnerParams.appWindowParams.windowTitle = "inkjet";
        ImmApp::AddOnsParams addOnsParams;
        addOnsParams.withMarkdown = true;
        addOnsParams.withImplot = true;
        ImmApp::Run(runnerParams, addOnsParams);

    }
    ~Application()=default;

    void render()
    {
        inkjet::BeginMainWindow();

        inkjet::DockSpace(false);


        inkjet::Begin(ICON_MD_TERMINAL" Client");
        if(ImGui::Button("Request a random number async (takes 1 sec)"))
        {
            mClient->requestAsync(Empty::empty);
        }
        if(ImGui::Button("Request a random number sync (takes 1 sec)"))
        {
            auto ret = mClient->requestSync(Empty::empty, std::chrono::milliseconds(3000));
            if(ret.has_value())
                mNumber = ret.value();
        }
        ImGui::Text("%s", std::to_string(mNumber).c_str());
        inkjet::End();

        inkjet::EndMainWindow();
    }
private:
    ServerThread mServerThread;
    Client<Empty, int>::SharedPtr mClient;
    int mNumber = 0;


};

int main(int , char *[])
{
    Application application;
}