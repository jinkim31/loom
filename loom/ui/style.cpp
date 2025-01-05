#include "style.h"

loom::ui::Style globalStyle;

void loom::ui::setStyle(const loom::ui::Style &style)
{
    globalStyle = style;
    ImGuiStyle* imStyle =  &ImGui::GetStyle();
    ImVec4* imColors = imStyle->Colors;

    imColors[ImGuiCol_Text]                   = style.text;
    imColors[ImGuiCol_TextDisabled]           = colorA(style.text, 100);
    imColors[ImGuiCol_WindowBg]               = style.foreground;
    imColors[ImGuiCol_ChildBg]                = style.foreground;
    imColors[ImGuiCol_PopupBg]                = style.foreground;
    imColors[ImGuiCol_Border]                 = style.border;
    imColors[ImGuiCol_BorderShadow]           = colorA(style.border, 0);
    imColors[ImGuiCol_FrameBg]                = style.background;
    imColors[ImGuiCol_FrameBgHovered]         = style.background;
    imColors[ImGuiCol_FrameBgActive]          = style.background;
    imColors[ImGuiCol_TitleBg]                = style.foreground;
    imColors[ImGuiCol_TitleBgActive]          = style.foreground;
    imColors[ImGuiCol_TitleBgCollapsed]       = style.foreground;
    imColors[ImGuiCol_MenuBarBg]              = style.foreground;
    imColors[ImGuiCol_ScrollbarBg]            = style.background;
    imColors[ImGuiCol_ScrollbarGrab]          = colorA(style.text, 70);
    imColors[ImGuiCol_ScrollbarGrabHovered]   = colorA(style.text, 80);
    imColors[ImGuiCol_ScrollbarGrabActive]    = colorA(style.text, 90);
    imColors[ImGuiCol_CheckMark]              = style.highlight;
    imColors[ImGuiCol_SliderGrab]             = colorA(style.highlight, 170);
    imColors[ImGuiCol_SliderGrabActive]       = colorA(style.highlight, 200);
    imColors[ImGuiCol_Button]                 = colorA(style.text, 20);
    imColors[ImGuiCol_ButtonHovered]          = colorA(style.text, 30);
    imColors[ImGuiCol_ButtonActive]           = colorA(style.text, 40);
    imColors[ImGuiCol_Header]                 = colorA(style.text, 20);
    imColors[ImGuiCol_HeaderHovered]          = colorA(style.text, 30);
    imColors[ImGuiCol_HeaderActive]           = colorA(style.text, 40);
    imColors[ImGuiCol_Separator]              = style.border;
    imColors[ImGuiCol_SeparatorHovered]       = style.border;
    imColors[ImGuiCol_SeparatorActive]        = style.border;
    imColors[ImGuiCol_ResizeGrip]             = colorA(style.highlight, 150);
    imColors[ImGuiCol_ResizeGripHovered]      = colorA(style.highlight, 150);
    imColors[ImGuiCol_ResizeGripActive]       = colorA(style.highlight, 220);
    imColors[ImGuiCol_Tab]                    = style.foreground;
    imColors[ImGuiCol_TabHovered]             = style.foreground;
    imColors[ImGuiCol_TabActive]              = style.foreground;
    imColors[ImGuiCol_TabUnfocused]           = style.foreground;
    imColors[ImGuiCol_TabUnfocusedActive]     = style.foreground;
    imColors[ImGuiCol_DockingPreview]         = colorA(style.highlight, 100);
    imColors[ImGuiCol_DockingEmptyBg]         = colorRGB(255, 0, 0);
    imColors[ImGuiCol_PlotLines]              = colorRGB(255, 0, 0);
    imColors[ImGuiCol_PlotLinesHovered]       = colorRGB(255, 0, 0);
    imColors[ImGuiCol_PlotHistogram]          = colorRGB(255, 0, 0);
    imColors[ImGuiCol_PlotHistogramHovered]   = colorRGB(255, 0, 0);
    imColors[ImGuiCol_TableHeaderBg]          = style.border;
    imColors[ImGuiCol_TableBorderStrong]      = style.border;
    imColors[ImGuiCol_TableBorderLight]       = colorA(style.border, 200);
    imColors[ImGuiCol_TableRowBg]             = style.background;
    imColors[ImGuiCol_TableRowBgAlt]          = style.foreground;
    imColors[ImGuiCol_TextSelectedBg]         = colorA(style.highlight, 120);
    imColors[ImGuiCol_DragDropTarget]         = style.highlight;
    imColors[ImGuiCol_NavHighlight]           = colorA(style.highlight, 200);
    imColors[ImGuiCol_NavWindowingHighlight]  = colorA(style.highlight, 200);
    imColors[ImGuiCol_NavWindowingDimBg]      = colorA(style.highlight, 200);
    imColors[ImGuiCol_ModalWindowDimBg]       = colorRGB(255, 0, 0);
    imColors[ImGuiCol_TabSelectedOverline]    = style.highlight;
    imColors[ImGuiCol_TabSelected]            = style.foreground;
    imColors[ImGuiCol_TabHovered]             = colorA(style.text, 10);

    imStyle->WindowMenuButtonPosition = ImGuiDir_Right;
    imStyle->WindowPadding = {12, 12};
    imStyle->FramePadding = {8, 8};
    imStyle->ItemSpacing = {12, 12};
    imStyle->ItemInnerSpacing = {4, 4};
    imStyle->WindowBorderSize = 1;
    imStyle->WindowRounding = 0;
    imStyle->CellPadding = {0, 0};
    imStyle->IndentSpacing = 16.0;
    imStyle->TabRounding = 0.0;
    imStyle->ChildRounding = 0.0;
    imStyle->WindowMinSize = {100, 100};
    imStyle->FrameRounding = 0;
    imStyle->FrameBorderSize = 0;
    imStyle->TabBarBorderSize = 0;
    imStyle->TabBarOverlineSize = 0;

    ImPlot::GetStyle().Colors[ImPlotCol_PlotBg] = style.background;
    ImPlot::GetStyle().Colors[ImPlotCol_FrameBg] = style.foreground;
    ImPlot::GetStyle().Colors[ImPlotCol_AxisGrid] = colorA(style.subtext, 120);
    ImPlot::GetStyle().PlotPadding = {0, 0};
    ImPlot::GetStyle().UseLocalTime = true;
}

const loom::ui::Style &loom::ui::getStyle()
{
    return globalStyle;
}

ImVec4 loom::ui::colorRGB(uint8_t r, uint8_t g, uint8_t b)
{ return {(float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f}; }

ImVec4 loom::ui::colorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{ return {(float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f}; }

ImVec4 loom::ui::colorA(const ImVec4 &color, uint8_t a)
{
    return ImVec4{color.x, color.y, color.z, color.w * (float)a / 255.0f};
}

loom::ui::Style loom::ui::lightStyle()
{
    loom::ui::Style style;
    style.highlight = colorRGB(41, 98, 255);
    style.background = colorRGB(255, 255, 255);
    style.foreground = colorRGB(240, 241, 242);
    style.border = colorRGB(212, 213, 214);
    style.text = colorRGB(29, 31, 33);
    style.subtext = colorA(style.text, 170);
    return style;
}

loom::ui::Style loom::ui::darkStyle()
{
    loom::ui::Style style;
    style.highlight = colorRGB(41, 98, 255);
    style.background = colorRGB(0, 0, 0);
    style.foreground = colorRGB(31, 33, 38);
    style.border = colorRGB(14, 15, 18);
    style.text = colorRGB(227, 228, 230);
    style.subtext = colorA(style.text, 220);
    return style;
}