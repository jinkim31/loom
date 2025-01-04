#include <loom/loom.h>

using namespace loom::ui;

class Application
{
public:
    Application()
    {
        HelloImGui::RunnerParams runnerParams;
        runnerParams.callbacks.SetupImGuiStyle = inkjet::setStyle;
        runnerParams.callbacks.ShowGui = [&]{render();};
        runnerParams.callbacks.BeforeExit = [&]{};
        runnerParams.imGuiWindowParams.defaultImGuiWindowType = HelloImGui::DefaultImGuiWindowType::NoDefaultWindow;
        runnerParams.callbacks.LoadAdditionalFonts = inkjet::loadFont;
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
        // begin main
        inkjet::BeginMainWindow();

        // menu bar
        inkjet::WidgetMenuBar([]{
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Create")) {
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                }
                if (ImGui::MenuItem("Save as..")) {
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Dummy")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                if (ImGui::MenuItem("Dummy")) {}
                ImGui::EndMenu();
            }
        }, []{
            ImGui::PushStyleColor(ImGuiCol_Text, inkjet::colorRGB(76, 175, 80));
            inkjet::TransparentButton(ICON_MD_PLAY_ARROW"##UtilityButton"); ImGui::SameLine();
            ImGui::PopStyleColor();
            char buffer[100] = "";
            inkjet::InputText("##UtillityText", "Utility Text", buffer, 100); ImGui::SameLine();
        });

        // dockspace
        inkjet::DockSpace();

        // status bar
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
        inkjet::HLine();
        ImGui::BeginChild("StatusBarChild");
        inkjet::TransparentButton("Status Bar Button");
        ImGui::EndChild();
        ImGui::PopStyleVar();

        inkjet::Begin(ICON_MD_TERMINAL" Terminal");
        inkjet::End();

        inkjet::Begin(ICON_MD_BAR_CHART" Plot");
        ImGui::PushStyleColor(ImGuiCol_FrameBg, inkjet::white);
        if(ImPlot::BeginPlot("Plot", ImGui::GetContentRegionAvail()))
            ImPlot::EndPlot();
        ImGui::PopStyleColor();
        inkjet::End();

        // end main
        inkjet::EndMainWindow();
    }
private:

};


int main(int , char *[])
{
    Application application;
}