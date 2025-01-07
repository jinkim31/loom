#ifndef LOOM_UI_UI_H
#define LOOM_UI_UI_H

#include "../external/IconFontCppHeaders/IconsMaterialDesign.h"
#include "style.h"

namespace loom::ui
{

static const ImVec4 green = ImColor(98, 197, 84);
static const ImVec4 red = ImColor(237, 106, 95);
static const ImVec4 yellow = ImColor(245, 191, 76);
static std::map<std::string, ImFont *> fonts;

void BeginMainWindow();

void EndMainWindow();

void DockSpace(bool leaveSpaceForStatusBar = true);

bool
Begin(const char *name, bool *open = NULL, bool usePadding = true, const std::function<void(void)> &ShowMenu = NULL);

void End();

void loadFont();

bool InputText(const char *label, const char *hint, char *buf, size_t buf_size, ImGuiInputTextFlags flags = 0);

void
WidgetMenuBar(const std::function<void()> &Menu, const std::function<void()> &Widget, const float &menuWidth = 300);

bool TransparentButton(const char *name, const ImVec2 &size = ImVec2(0, 0));

void HLine(float margin = 0.0);

void VLine();

void TableLabel(const char *text);

bool Combo(const char *label, int *index, const std::vector<std::string> &items);

enum LEDColor
{
    RED,
    GREEN,
    YELLOW,
    OFF,
};

void LED(LEDColor ledColor, const ImVec2 &size);

bool ButtonText(const char *text, const ImVec2 &size);

bool InputTextStdString(const char *label, std::string *str, const std::string &hint = "",
                        ImGuiInputTextFlags flags = ImGuiInputTextFlags_None,
                        ImGuiInputTextCallback callback = NULL, void *user_data = NULL);
bool InputTextMultiLineStdString(const char *label, std::string *str, const ImVec2 &size,
                                 ImGuiInputTextFlags flags = ImGuiInputTextFlags_None,
                                 ImGuiInputTextCallback callback = NULL, void *user_data = NULL);

bool CheckBox(char *text, bool *value);

bool BeginRightAlign(const char* id="rightAlign");

void EndRightAlign();

}


#endif