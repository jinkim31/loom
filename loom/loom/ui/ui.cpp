#include "ui.h"

void loom::ui::BeginMainWindow()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, getStyle().border);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
    ImGui::Begin("MAIN", NULL,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize
                 | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::PopStyleColor(1);
    ImGui::PopStyleVar(2);
}

void loom::ui::EndMainWindow()
{
    ImGui::End();
}

void loom::ui::HLine(float margin)
{
    auto draw_list = ImGui::GetCurrentWindow()->DrawList;
    draw_list->AddLine(
            ImGui::GetCursorScreenPos() + ImVec2{0, 0},
            ImGui::GetCursorScreenPos() + ImVec2(ImGui::GetContentRegionAvail().x, 0),
            ImGui::GetColorU32(getStyle().border));
    ImGui::Dummy({0, 1});
}

void loom::ui::VLine()
{
    auto draw_list = ImGui::GetCurrentWindow()->DrawList;
    draw_list->AddLine(
            ImGui::GetCursorScreenPos() + ImVec2(0, 4),
            ImGui::GetCursorScreenPos() + ImVec2(0, ImGui::GetFrameHeight()-4),
            ImGui::GetColorU32(ImGuiCol_Border));
    ImGui::Dummy({1, 0});
}

void loom::ui::DockSpace(bool leaveSpaceForStatusBar)
{
    const int padding = 0;
    ImGui::PushStyleVar(ImGuiStyleVar_DockingSeparatorSize, 1);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, {0, 4});
    //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 8});
    ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyleColorVec4(ImGuiCol_Border));
    ImGui::PushStyleColor(ImGuiCol_Text, getStyle().subtext);
    ImGui::Dummy({0, padding});
    ImGui::Dummy({padding, 0});
    ImGui::SameLine();
    ImGui::DockSpace(ImGui::GetID("MainDockSpace"),
                     ImVec2(-padding, -padding - (leaveSpaceForStatusBar ? ImGui::GetFrameHeight()+1 : 0)),
                     ImGuiDockNodeFlags_PassthruCentralNode|ImGuiDockNodeFlags_NoCloseButton|ImGuiDockNodeFlags_NoWindowMenuButton);
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);
}

bool loom::ui::TransparentButton(const char* name, const ImVec2& size)
{
    ImGui::PushStyleColor(ImGuiCol_Button, colorRGBA(0, 0, 0, 0));
    bool ret = ImGui::Button(name, size);
    ImGui::PopStyleColor();
    return ret;
}

bool loom::ui::Begin(const char* name, bool* open, bool usePadding, const std::function<void(void)>& ShowMenu)
{
    // returns true if focused

    // check tab active
    ImGuiWindow* window = ImGui::FindWindowByName(name);
    bool isSelected = window != nullptr && !window->Hidden;

    // background color
    //ImGui::PushStyleColor(ImGuiCol_WindowBg, getStyle().background);

    // check if window is focused
    ImGuiID id = ImGui::GetID(name);
    static std::map<ImGuiID, bool> windowFocusedMap;
    bool isFocused = false;
    auto windowFocusedIter = windowFocusedMap.find(id);
    if(windowFocusedIter != windowFocusedMap.end() && windowFocusedIter->second)
        isFocused = true;

    // apply tab hover color
    bool isHovering = window != nullptr && ImGui::IsMouseHoveringRect(window->DockTabItemRect.Min, window->DockTabItemRect.Max);
    ImGui::PushStyleColor(ImGuiCol_Text, isSelected
    ? (isFocused ? getStyle().highlight : getStyle().text)
    : (isHovering ? getStyle().subtext : getStyle().subtext));

    // zero window padding for tab border
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});

    // begin window
    ImGuiWindowFlags flags =  ImGuiWindowFlags_NoScrollbar;
    ImGui::Begin(name, open, flags);

    // update focused map
    windowFocusedMap[id] = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);

    // pop title color
    //ImGui::PopStyleColor();

    // tab border without spacing below
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0,0});

    // line
    HLine();

    ImGui::PopStyleVar();

    ImGui::PopStyleColor();

    // push child window background color to be same as the window background
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));

    ImGui::BeginChild("windowChild", ImGui::GetContentRegionAvail(),
                      ImGuiChildFlags_None,
                      (ShowMenu!=NULL) ? ImGuiWindowFlags_MenuBar : ImGuiWindowFlags_None);

    ImGui::PopStyleVar();

    if(ShowMenu)
    {
        if(ImGui::BeginMenuBar())
        {
            ShowMenu();
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0,0});
            ImGui::EndMenuBar();
            HLine();
            ImGui::PopStyleVar();
        }
    }

    if(usePadding)
    {

        ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));
        ImGui::BeginChild("paddingChild", ImGui::GetContentRegionAvail(),
                          ImGuiChildFlags_Border,
                          ImGuiWindowFlags_None);
        ImGui::PopStyleColor();
    }
    else
    {
        ImGui::BeginChild("paddingChild", ImGui::GetContentRegionAvail(),
                          ImGuiChildFlags_None,
                          ImGuiWindowFlags_None);
    }

    return true;
}

void loom::ui::End()
{
    ImGui::EndChild();

    // end child
    ImGui::EndChild();

    ImGui::PopStyleColor(1);

    // end window
    ImGui::End();
}

bool loom::ui::InputText(const char *label, const char* hint, char *buf, size_t buf_size, ImGuiInputTextFlags flags)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, getStyle().background);
    bool ret = ImGui::InputTextWithHint(label, hint, buf, buf_size, flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    return ret;
}

void loom::ui::WidgetMenuBar(const std::function<void()>& Menu, const std::function<void()>& Widget, const float& menuWidth)
{
    // top line to distinguish "Windows" titlebar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    HLine();
    ImGui::PopStyleVar();

    // menu child
    ImGui::BeginChild("MenuChild", {menuWidth, ImGui::GetFrameHeight()}, ImGuiChildFlags_None, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) {
        Menu();
        ImGui::EndMenuBar();
    }
    ImGui::EndChild();

    // same line in between
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    ImGui::SameLine();

    // utility child
    ImGui::BeginChild("UtilityChild", {0, ImGui::GetFrameHeight()}, ImGuiChildFlags_None, ImGuiWindowFlags_None);

    Widget();

    // utility child end and line
    ImGui::EndChild();
    HLine();
    ImGui::PopStyleVar();
}

bool loom::ui::Combo(const char *label, int *index, const std::vector<std::string> &items)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {16-1, 16-1});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {8, 8});
    const char* preview = (*index < items.size()) ? items[*index].c_str() : "";
    if (ImGui::BeginCombo(label, preview, ImGuiComboFlags_None))
    {
        for (int n = 0; n < items.size(); n++)
        {
            bool is_selected = items.size() <= *index || (items[*index] == items[n]);
            if (ImGui::Selectable(items[n].c_str(), is_selected))
                *index = n;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::PopStyleVar(2);
    return true;
}

void loom::ui::LED(LEDColor ledColor, const ImVec2 &size)
{
    ImColor color;
    switch (ledColor)
    {
        case GREEN:
            color = ImColor(98,197,84);
            break;
        case RED:
            color = ImColor(237,106,95);
            break;
        case YELLOW:
            color = ImColor(245,191,76);
            break;
    }
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    int width = 8;
    int height = ImGui::GetFrameHeight();

    draw_list->AddCircleFilled({ImGui::GetCursorScreenPos().x + width/2, ImGui::GetCursorScreenPos().y + height/2},
                               width/2.0, color);
    ImGui::Dummy(ImVec2(width, height));
}

bool loom::ui::ButtonText(const char *text,  const ImVec2& size)
{
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorRGBA(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorRGBA(0, 0, 0, 0));
    bool ret = TransparentButton(text, size);
    ImGui::PopStyleColor(2);
    return ret;
}

struct InputTextCallback_UserData
{
    std::string*            Str;
    ImGuiInputTextCallback  ChainCallback;
    void*                   ChainCallbackUserData;
};

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
    InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        // Resize string callback
        // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
        std::string* str = user_data->Str;
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);
        data->Buf = (char*)str->c_str();
    }
    else if (user_data->ChainCallback)
    {
        // Forward to user callback, if any
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
    }
    return 0;
}

bool loom::ui::InputTextStdString(const char *label, std::string *str, const std::string& hint, ImGuiInputTextFlags flags,
                                ImGuiInputTextCallback callback, void *user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    //return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    return ImGui::InputTextWithHint(label, (char*)hint.c_str(), (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
}

bool loom::ui::InputTextMultiLineStdString(const char *label, std::string *str, const ImVec2& size,
                                         ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void *user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return ImGui::InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
}

void loom::ui::TableLabel(const char *text)
{
    //ImGui::Dummy(ImGui::GetStyle().ItemSpacing); ImGui::SameLine();
    ImGui::AlignTextToFramePadding();
    ImGui::Text("%s", text); ImGui::SameLine();
    //ImGui::Dummy(ImGui::GetStyle().ItemSpacing);
}

bool loom::ui::CheckBox(char *text, bool *value)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {4,4});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, {0, 0});
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
    ImGui::SetCursorPosY(4);
    //ImGui::Dummy({16, 16});
    bool ret = ImGui::Checkbox(text, value);
    ImGui::PopStyleVar(3);
    return ret;
}

std::map<ImGuiID, float> rightAlignSpaceMap;

bool loom::ui::BeginRightAlign(const char *id)
{
    ImGui::PushID(id);
    auto rightAlignSpace = rightAlignSpaceMap.find(ImGui::GetID("alignSpace"));
    if(rightAlignSpace != rightAlignSpaceMap.end())
    {
        ImGui::Dummy({rightAlignSpace->second, 0});
        ImGui::SameLine();
    }
    return true;
}

void loom::ui::EndRightAlign()
{
    ImGui::SameLine();
    rightAlignSpaceMap[ImGui::GetID("alignSpace")] += ImGui::GetContentRegionAvail().x;
    ImGui::PopID();
}

void loom::ui::loadFont()
{
    float fontSize = 16.0f;

    HelloImGui::FontLoadingParams mainFontLoadingParams;
    mainFontLoadingParams.useFullGlyphRange = true;
    HelloImGui::LoadFont("fonts/SpaceGrotesk/SpaceGrotesk-Regular.ttf" , fontSize, mainFontLoadingParams);

    HelloImGui::FontLoadingParams iconFontLoadingParams;
    iconFontLoadingParams.mergeToLastFont = true;
    iconFontLoadingParams.useFullGlyphRange = true;
    iconFontLoadingParams.fontConfig.GlyphOffset = {0, 4 * HelloImGui::DpiFontLoadingFactor()};
    //HelloImGui::LoadFont("fonts/MaterialIcons-Regular.ttf" , fontSize, iconFontLoadingParams);
    HelloImGui::LoadFont("fonts/MaterialSymbolsRounded-Light.ttf" , fontSize * 1.4, iconFontLoadingParams);
}
