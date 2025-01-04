#include "inkjet.h"

void loom::inkjet::BeginMainWindow()
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, background);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
    ImGui::Begin("MAIN", NULL,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize
                 | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::PopStyleColor(1);
    ImGui::PopStyleVar(2);
}

void loom::inkjet::EndMainWindow()
{
    ImGui::End();
}

void loom::inkjet::HLine(float margin)
{
    auto draw_list = ImGui::GetCurrentWindow()->DrawList;
    draw_list->AddLine(
            ImGui::GetCursorScreenPos() + ImVec2{margin, 0},
            ImGui::GetCursorScreenPos() + ImVec2(ImGui::GetContentRegionAvail().x-margin, 0),
            ImGui::GetColorU32(ImGuiCol_Border));
    ImGui::Dummy({0, 1});
}

void loom::inkjet::VLine()
{
    auto draw_list = ImGui::GetCurrentWindow()->DrawList;
    draw_list->AddLine(
            ImGui::GetCursorScreenPos() + ImVec2(0, 4),
            ImGui::GetCursorScreenPos() + ImVec2(0, ImGui::GetFrameHeight()-4),
            ImGui::GetColorU32(ImGuiCol_Border));
    ImGui::Dummy({1, 0});
}

void loom::inkjet::DockSpace(bool leaveSpaceForStatusBar)
{
    const int padding = 0;
    const int dockSpacing = 1;
    ImGui::PushStyleVar(ImGuiStyleVar_DockingSeparatorSize, dockSpacing);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, {0, 4});
    ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyleColorVec4(ImGuiCol_Border));
    ImGui::PushStyleColor(ImGuiCol_Text, textSubtitle);
    ImGui::Dummy({0, padding});
    ImGui::Dummy({padding, 0});
    ImGui::SameLine();
    ImGui::DockSpace(ImGui::GetID("MainDockSpace"),
                     ImVec2(-padding, -padding - (leaveSpaceForStatusBar ? ImGui::GetFrameHeight()+1 : 0)),
                     ImGuiDockNodeFlags_PassthruCentralNode|ImGuiDockNodeFlags_NoCloseButton|ImGuiDockNodeFlags_NoWindowMenuButton);
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);
}

void loom::inkjet::setStyle()
{
    ImGuiStyle* style =  &ImGui::GetStyle();
    ImVec4* colors = style->Colors;


    colors[ImGuiCol_Text]                   = colorRGB(34, 34, 34);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg]               = panel;
    colors[ImGuiCol_ChildBg]                = panel;
    colors[ImGuiCol_PopupBg]                = panel;
    colors[ImGuiCol_Border]                 = border;
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = white;
    colors[ImGuiCol_FrameBgHovered]         = colorRGBA(220, 220, 220, 120);
    colors[ImGuiCol_FrameBgActive]          = colorRGBA(220, 220, 220, 170);
    colors[ImGuiCol_TitleBg]                = panel;
    colors[ImGuiCol_TitleBgActive]          = panel;
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = panel;
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
    colors[ImGuiCol_CheckMark]              = colorRGB(34, 34, 34);
    colors[ImGuiCol_SliderGrab]             = colorRGB(170, 170, 170);
    colors[ImGuiCol_SliderGrabActive]       = highlight;
    colors[ImGuiCol_Button]                 = colorRGBA(210, 210, 210, 70);
    colors[ImGuiCol_ButtonHovered]          = colorRGBA(220, 220, 220, 120);
    colors[ImGuiCol_ButtonActive]           = colorRGBA(220, 220, 220, 170);
    colors[ImGuiCol_Header]                 = colorRGBA(220, 220, 220, 70);
    colors[ImGuiCol_HeaderHovered]          = colorRGBA(220, 220, 220, 120);
    colors[ImGuiCol_HeaderActive]           = colorRGBA(220, 220, 220, 170);
    colors[ImGuiCol_Separator]              = colorRGBA(220, 220, 220, 70);
    colors[ImGuiCol_SeparatorHovered]       = colorRGBA(220, 220, 220, 120);
    colors[ImGuiCol_SeparatorActive]        = colorRGBA(220, 220, 220, 150);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab]                    = panel;
    colors[ImGuiCol_TabHovered]             = panel;
    colors[ImGuiCol_TabActive]              = panel;
    colors[ImGuiCol_TabUnfocused]           = panel;
    colors[ImGuiCol_TabUnfocusedActive]     = panel;
    //colors[ImGuiCol_DockingPreview]         = colors[ImGuiCol_Header] * ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = colorRGB(229, 232, 237);
    colors[ImGuiCol_TableBorderStrong]      = border;
    colors[ImGuiCol_TableBorderLight]       = border;
    colors[ImGuiCol_TableRowBg]             = white;
    colors[ImGuiCol_TableRowBgAlt]          = colorRGB(248, 248, 248);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_NavHighlight]           = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    colors[ImGuiCol_TabSelectedOverline]    = highlight;
    colors[ImGuiCol_TabSelected]            = colorRGB(232, 234, 240);
    colors[ImGuiCol_TabHovered]             = colorRGBA(220, 220, 220, 120);;

    style->WindowMenuButtonPosition = ImGuiDir_Right;
    style->WindowPadding = {12, 12};
    style->FramePadding = {8, 8};
    style->ItemSpacing = {12, 12};
    style->ItemInnerSpacing = {4, 4};
    style->WindowBorderSize = 1;
    style->WindowRounding = 0;
    style->CellPadding = {0, 0};
    style->IndentSpacing = 16.0;
    style->TabRounding = 0.0;
    style->ChildRounding = 0.0;
    style->WindowMinSize = {100, 100};
    style->FrameRounding = 0;
    style->FrameBorderSize = 0;
    style->TabBarBorderSize = 0;
    style->TabBarOverlineSize = 2;

    ImPlot::GetStyle().Colors[ImPlotCol_PlotBg] = white;
    ImPlot::GetStyle().Colors[ImPlotCol_FrameBg] = colorRGBA(0,0,0,0);
    ImPlot::GetStyle().PlotPadding = {0, 0};
    ImPlot::GetStyle().UseLocalTime = true;
}

bool loom::inkjet::TransparentButton(const char* name, const ImVec2& size)
{
    ImGui::PushStyleColor(ImGuiCol_Button, inkjet::colorRGBA(0, 0, 0, 0));
    bool ret = ImGui::Button(name, size);
    ImGui::PopStyleColor();
    return ret;
}

bool loom::inkjet::Begin(const char* name, bool* open, bool usePadding, const std::function<void(void)>& ShowMenu)
{
    // returns true if focused

    // check tab active
    ImGuiWindow* window = ImGui::FindWindowByName(name);
    bool isActive = window != nullptr && !window->Hidden;

    // background color
    ImGui::PushStyleColor(ImGuiCol_WindowBg, panel);

    // apply tab hover color
    bool isHovering = window != nullptr && ImGui::IsMouseHoveringRect(window->DockTabItemRect.Min, window->DockTabItemRect.Max);
    ImGui::PushStyleColor(ImGuiCol_Text, isActive ? highlight : (isHovering ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : textSubtitle));

    // zero window padding for tab border
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});

    // begin window
    ImGuiWindowFlags flags =   ImGuiWindowFlags_NoScrollbar;
    ImGui::Begin(name, open, flags);

    // pop title color
    ImGui::PopStyleColor();

    // tab border without spacing below
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0,0});
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
            inkjet::HLine();
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

    return ImGui::IsWindowFocused();
}

void loom::inkjet::End()
{
    ImGui::EndChild();

    // end child
    ImGui::EndChild();

    ImGui::PopStyleColor(1);

    // end window
    ImGui::End();
}

void loom::inkjet::initFont()
{
    HelloImGui::FontLoadingParams mainFontLoadingParams;
    mainFontLoadingParams.useFullGlyphRange = true;
    HelloImGui::LoadFont("fonts/SpaceGrotesk/SpaceGrotesk-Regular.ttf" , 16.0f, mainFontLoadingParams);
    HelloImGui::FontLoadingParams iconFontLoadingParams;
    iconFontLoadingParams.mergeToLastFont = true;
    iconFontLoadingParams.useFullGlyphRange = true;
    iconFontLoadingParams.fontConfig.GlyphOffset = {0, 4 * HelloImGui::DpiFontLoadingFactor()};
    HelloImGui::LoadFont("fonts/MaterialIcons-Regular.ttf" , 16.0f, iconFontLoadingParams);
}

bool loom::inkjet::InputText(const char *label, const char* hint, char *buf, size_t buf_size, ImGuiInputTextFlags flags)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, inkjet::white);
    bool ret = ImGui::InputTextWithHint(label, hint, buf, buf_size, flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    return ret;
}

void loom::inkjet::WidgetMenuBar(const std::function<void()>& Menu, const std::function<void()>& Widget, const float& menuWidth)
{
    // top line to distinguish Windows titlebar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    inkjet::HLine();
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
    inkjet::HLine();
    ImGui::PopStyleVar();
}

bool loom::inkjet::Combo(const char *label, int *index, const std::vector<std::string> &items)
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

void loom::inkjet::LED(inkjet::LEDColor ledColor, const ImVec2 &size)
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

bool loom::inkjet::ButtonText(const char *text,  const ImVec2& size)
{
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorRGBA(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorRGBA(0, 0, 0, 0));
    bool ret = inkjet::TransparentButton(text, size);
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

bool loom::inkjet::InputTextStdString(const char *label, std::string *str, const std::string& hint, ImGuiInputTextFlags flags,
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

bool loom::inkjet::InputTextMultiLineStdString(const char *label, std::string *str, const ImVec2& size,
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

void loom::inkjet::TableLabel(const char *text)
{
    //ImGui::Dummy(ImGui::GetStyle().ItemSpacing); ImGui::SameLine();
    ImGui::AlignTextToFramePadding();
    ImGui::Text("%s", text); ImGui::SameLine();
    //ImGui::Dummy(ImGui::GetStyle().ItemSpacing);
}

bool loom::inkjet::CheckBox(char *text, bool *value)
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
