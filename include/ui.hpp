#pragma once

#include <cstdint>
#include <string_view>

namespace ui {
extern void box(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
extern void header(std::string_view str);
extern void footer(std::string_view str);
} // namespace ui
