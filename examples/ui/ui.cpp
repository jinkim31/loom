#include <loom/loom.h>

using namespace loom;

class Application
{
public:
    Application()
    {
        HelloImGui::RunnerParams runnerParams;
        runnerParams.callbacks.SetupImGuiStyle = []{ui::setStyle(ui::lightStyle());};
        runnerParams.callbacks.ShowGui = [&]{render();};
        runnerParams.callbacks.BeforeExit = [&]{};
        runnerParams.imGuiWindowParams.defaultImGuiWindowType = HelloImGui::DefaultImGuiWindowType::NoDefaultWindow;
        runnerParams.callbacks.LoadAdditionalFonts = ui::loadFont;
        runnerParams.imGuiWindowParams.enableViewports = true;
        runnerParams.fpsIdling.enableIdling = false;
        runnerParams.appWindowParams.windowTitle = "ui";
        ImmApp::AddOnsParams addOnsParams;
        addOnsParams.withMarkdown = true;
        addOnsParams.withImplot = true;

        ImmApp::Run(runnerParams, addOnsParams);
    }
    ~Application()=default;

    void render()
    {
        // begin main
        ui::BeginMainWindow();

        // menu bar
        ui::WidgetMenuBar([]{
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
            ImGui::PushStyleColor(ImGuiCol_Text, ui::colorRGB(76, 175, 80));
            ui::TransparentButton(ICON_MD_PLAY_ARROW"##UtilityButton"); ImGui::SameLine();
            ImGui::PopStyleColor();
            char buffer[100] = "";
            ui::InputText("##UtillityText", "Utility Text", buffer, 100); ImGui::SameLine();
        });

        // dockspace
        ui::DockSpace();

        // status bar
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
        ui::HLine();
        ImGui::BeginChild("StatusBarChild");
        ui::TransparentButton("Status Bar Button");
        ImGui::EndChild();
        ImGui::PopStyleVar();

        ui::Begin(ICON_MD_TERMINAL" Terminal");
        ui::End();

        ui::Begin(ICON_MD_BAR_CHART" Plot");
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ui::getStyle().background);
        if(ImPlot::BeginPlot("Plot", ImGui::GetContentRegionAvail()))
            ImPlot::EndPlot();
        ImGui::PopStyleColor();
        ui::End();

        ui::Begin(ICON_MD_BRUSH" Style Editor");
        ImGui::ShowStyleEditor();
        ui::End();

        // end main
        ui::EndMainWindow();
    }
private:

};


int main(int , char *[])
{
    Application application;
}