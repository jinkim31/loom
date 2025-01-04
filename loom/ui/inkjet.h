#ifndef LOOM_UI_INKJET
#define LOOM_UI_INKJET

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <immapp/immapp.h>
#include <imgui_internal.h>
#include <implot/implot.h>
#include <unordered_map>
#include "../../external/IconFontCppHeaders/IconsMaterialDesign.h"



namespace loom::ui::inkjet
{

static ImVec4 colorRGB(uint8_t r, uint8_t g, uint8_t b)
{ return {r / 255.0f, g / 255.0f, b / 255.0f, 1.0f}; }

static ImVec4 colorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{ return {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f}; }

static const ImVec4 white = colorRGB(255, 255, 255);
static const ImVec4 background = colorRGB(250, 250, 250);
static const ImVec4 panel = colorRGB(242, 242, 242);
static const ImVec4 highlight = colorRGB(41, 98, 255);
static const ImVec4 textSubtitle = colorRGB(105, 105, 105);
static const ImVec4 border = colorRGB(211, 211, 211);
static const ImVec4 green = ImColor(98, 197, 84);
static const ImVec4 red = ImColor(237, 106, 95);
static const ImVec4 yellow = ImColor(245, 191, 76);
static std::map<std::string, ImFont *> fonts;

void BeginMainWindow();

void EndMainWindow();

void DockSpace(bool leaveSpaceForStatusBar = true);

void setStyle();

bool
Begin(const char *name, bool *open = NULL, bool usePadding = true, const std::function<void(void)> &ShowMenu = NULL);

void End();

void initFont();

void loadFont()
{
    float fontSize = 16.0f;

    HelloImGui::FontLoadingParams mainFontLoadingParams;
    mainFontLoadingParams.useFullGlyphRange = true;
    HelloImGui::LoadFont("fonts/SpaceGrotesk/SpaceGrotesk-Regular.ttf" , fontSize, mainFontLoadingParams);

    HelloImGui::FontLoadingParams iconFontLoadingParams;
    iconFontLoadingParams.mergeToLastFont = true;
    iconFontLoadingParams.useFullGlyphRange = true;
    iconFontLoadingParams.fontConfig.GlyphOffset = {0, 4 * HelloImGui::DpiFontLoadingFactor()};
    HelloImGui::LoadFont("fonts/MaterialIcons-Regular.ttf" , fontSize, iconFontLoadingParams);
}

bool InputText(const char *label, const char *hint, char *buf, size_t buf_size, ImGuiInputTextFlags flags = 0);

void
WidgetMenuBar(const std::function<void()> &Menu, const std::function<void()> &Widget, const float &menuWidth = 300);

bool TransparentButton(const char *name, const ImVec2 &size = ImVec2(0, 0));

void HLine(float margin = 0.0);

void VLine();

void TableLabel(const char *text);
#ifdef INKJET_WITH_IMMVISION
void ImageView(char* name, cv::Mat mat, ImmVision::ImageParams& param, bool home, bool refresh);
#endif

bool Combo(const char *label, int *index, const std::vector<std::string> &items);

enum LEDColor
{
    RED,
    GREEN,
    YELLOW,
    OFF,
};

void LED(inkjet::LEDColor ledColor, const ImVec2 &size);

bool ButtonText(const char *text, const ImVec2 &size);

bool InputTextStdString(const char *label, std::string *str, const std::string &hint = "",
                        ImGuiInputTextFlags flags = ImGuiInputTextFlags_None,
                        ImGuiInputTextCallback callback = NULL, void *user_data = NULL);
bool InputTextMultiLineStdString(const char *label, std::string *str, const ImVec2 &size,
                                 ImGuiInputTextFlags flags = ImGuiInputTextFlags_None,
                                 ImGuiInputTextCallback callback = NULL, void *user_data = NULL);

bool CheckBox(char *text, bool *value);

}


#endif