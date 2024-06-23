#include "analyticsexception.h"
#include "utils.h"

namespace
{
    static std::unordered_map<uint8_t, QColor> colors;

    void initColors() {
        colors.emplace(0, "#00ffff");
        colors.emplace(1, "#ff0000");
        colors.emplace(2, "#00ff00");
        colors.emplace(3, "#ffa500");
        colors.emplace(4, "#9370db");
        colors.emplace(5, "#96D996");
        colors.emplace(6, "#D39B85");
        colors.emplace(7, "#E13AFF");
        colors.emplace(8, "#008000");
        colors.emplace(9, "#FFFF00");
        colors.emplace(10, "#006464");
        colors.emplace(11, "#323232");
        colors.emplace(12, "#B1FF4C");
        colors.emplace(13, "#6C3B2A");
        colors.emplace(14, "#B0C4DE");
        colors.emplace(15, "#BEBD7F");
        colors.emplace(16, "#B85D43");
        colors.emplace(17, "#122FAA");
        colors.emplace(18, "#FCD975");
        colors.emplace(19, "#8B8940");
        colors.emplace(20, "#78DBE2");
        colors.emplace(21, "#A0522D");
        colors.emplace(22, "#122FAA");
    }
}

QColor utils::colorByClass(uint8_t cellClass)
{
    if (colors.empty())
        initColors();

    auto it = colors.find(cellClass);
    if (it == colors.end())
        throw AnalyticsException("Can't find color for class " + std::to_string(cellClass));

    return it->second;
}
