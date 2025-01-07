#ifndef LOOM_UI_STYLE_H
#define LOOM_UI_STYLE_H

#define IMGUI_DEFINE_MATH_OPERATORS
#include <immapp/immapp.h>
#include <imgui_internal.h>
#include <implot/implot.h>

namespace loom::ui
{

ImVec4 colorRGB(uint8_t r, uint8_t g, uint8_t b);
ImVec4 colorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
ImVec4 colorA(const ImVec4& color, uint8_t a);

struct Style
{
    ImVec4 background ;
    ImVec4 foreground;
    ImVec4 highlight;
    ImVec4 text;
    ImVec4 subtext;
    ImVec4 border;
};

void setStyle(const Style &style);
const Style &getStyle();

Style lightStyle();
Style darkStyle();

}

#endif
