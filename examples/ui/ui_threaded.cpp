#include <loom/loom.h>
#include <cstdlib>
#include <ctime>

using namespace loom;

class ServerThread : public loom::LoopingThread
{
public:
    ServerThread()
    {
        std::srand(std::time(nullptr));

        server = makeServer<loom::Empty, int>([](const loom::Empty&){
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            return std::rand();
        });
    }
    loom::Server<loom::Empty, int>::SharedPtr server;
};

class Application : public loom::ManualThread
{
public:
    Application()
    {
        HelloImGui::RunnerParams runnerParams;
        runnerParams.callbacks.SetupImGuiStyle = ui::setStyle;
        runnerParams.callbacks.ShowGui = [&]{
            step();
            render();
        };
        runnerParams.callbacks.BeforeExit = [&]{
            mServerThread.stop();
            mServerThread.server->unlink(mClient);
        };

        runnerParams.callbacks.LoadAdditionalFonts = ui::loadFont;
        runnerParams.imGuiWindowParams.defaultImGuiWindowType = HelloImGui::DefaultImGuiWindowType::NoDefaultWindow;
        runnerParams.imGuiWindowParams.enableViewports = false;
        runnerParams.fpsIdling.enableIdling = false;
        runnerParams.appWindowParams.windowTitle = "ui";
        ImmApp::AddOnsParams addOnsParams;
        addOnsParams.withMarkdown = true;
        addOnsParams.withImplot = true;

        // threading
        mClient = makeClient<loom::Empty, int>([&](const int& number){
            mNumber = number; // callback for async request
        });
        mServerThread.start();
        mServerThread.server->link(mClient);

        ImmApp::Run(runnerParams, addOnsParams);
    }
    ~Application()=default;

    void render()
    {
        ui::BeginMainWindow();

        ui::DockSpace(false);


        ui::Begin(ICON_MD_TERMINAL" Client");
        if(ImGui::Button("Request a random number async (takes 0.5 sec)"))
        {
            mClient->requestAsync(loom::Empty::empty);
        }
        if(ImGui::Button("Request a random number sync (takes 0.5 sec)"))
        {
            auto ret = mClient->requestSync(loom::Empty::empty);
            if(ret.has_value())
                mNumber = ret.value();
        }
        ImGui::Text("%s", std::to_string(mNumber).c_str());
        ui::End();

        ui::EndMainWindow();
    }
private:
    ServerThread mServerThread;
    loom::Client<loom::Empty, int>::SharedPtr mClient;
    int mNumber = 0;
};

int main(int , char *[])
{
    Application application;
}