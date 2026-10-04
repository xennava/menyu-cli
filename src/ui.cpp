#include "terminal.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>
#include <ui.hpp>

namespace ui {
void box(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  int mx = x + w - 1;
  int my = y + h - 1;

  tui::put(x, y, "┌");
  tui::put(mx, y, "┐");
  tui::put(x, my, "└");
  tui::put(mx, my, "┘");

  for (size_t i = 1; i < w - 1; i++) {
    size_t cx = x + i;
    tui::put(cx, y, "─");
    tui::put(cx, my, "─");
  }
  for (size_t i = 1; i < h - 1; i++) {
    uint16_t cy = y + i;
    tui::put(x, cy, "\u2502");
    tui::put(mx, cy, "\u2502");
  }
}
void header(std::string_view str) {
  box(0, 0, tui::screenExtent.width - 1, 3);
  // print area
  tui::puts((tui::screenExtent.width - str.size()) >> 1, 1, str);
}
void footer(std::string_view str) {
  box(0, tui::screenExtent.height - 3, tui::screenExtent.width - 1, 3);
  tui::puts(3, 14, str);
}
} // namespace ui
