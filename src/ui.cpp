#include "terminal.hpp"
#include "type.hpp"
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

namespace view {
uint8_t idCtr = 0;
uint8_t activeID = 0;
uint8_t defaultActiveID = 0;
std::unordered_map<int, View *> viewRef;
void makeUniqueActiveView() {
  for (const auto &v : viewRef) {
    if (v.second->getID() != defaultActiveID) {
      v.second->state.activeView = 0;
    }
  }
}
void toggleActiveView() {
  viewRef[activeID]->state.activeView =
      viewRef[activeID]->state.activeView ? 0 : 1;
}
void stepActiveView(int bipolar) {
  if (idCtr < 1) {
    return;
  }
  if (bipolar == 0)
    return;
  toggleActiveView();

  if (bipolar < 0 && activeID == 0) {
    activeID = idCtr;
    goto end;
  }
  if (bipolar != 0) {
    if (activeID == idCtr - 1 && bipolar == 1)
      activeID = 0;
    else
      activeID += bipolar;
  }

end:
  toggleActiveView();
}
} // namespace view

void list(View &v, int &i, std::string_view str, ListProperty &lp) {

  vec2i xorg = v.getContentOrigin();
  Extent ext = v.getContentExtent();
  xorg.x += lp.margin.left;
  xorg.y += lp.margin.top;
  ext.width -= lp.margin.right;
  if (v.state.activeView) {
    if (lp.modes.autoMarkAll || lp.modes.autoMarkSelected) {
      tui::put(xorg.x,
               xorg.y +
                   (lp.modes.autoMarkSelected ? lp.marker.selectedElement : i),
               lp.marker.glyph.data());
      xorg.x += lp.marker.gap + 1;
      ext.width -= lp.marker.gap - 1;
    }
  }
  int intersection = str.size() - 1;
  int line = 0;
  // printf("x, y: %d, %d | content x, y: %d, %d | marked x, y: %d, %d\n",
  //        v.getOrigin().x, v.getOrigin().y, v.getContentOrigin().x,
  //        v.getContentOrigin().y, xorg.x, xorg.y);
  // printf("%s\n", str.substr(0, 1).data());

  do {
    intersection -= ext.width;
    tui::puts(xorg.x, xorg.y + i + line, str.data());
    line++;
    // strStart = intersection;
  } while (intersection > v.getContentExtent().width);
}

void list(View &v, int &i, std::string_view str, uint8_t selectedElement,
          uint8_t modes, std::string_view marker) {
  vec2i xorg = v.getContentOrigin();
  Extent ext = v.getContentExtent();
  if (modes == 0x1 || modes == 0x2) {
    tui::put(xorg.x, xorg.y + (modes == 0x2 ? selectedElement : i),
             marker.data());
    xorg.x += 2;
    ext.width -= 2;
  }
  int intersection = str.size() - 1;
  int line = 0;
  printf("x, y: %d, %d | content x, y: %d, %d | marked x, y: %d, %d\n",
         v.getOrigin().x, v.getOrigin().y, v.getContentOrigin().x,
         v.getContentOrigin().y, xorg.x, xorg.y);
  printf("%s\n", str.substr(0, 1).data());

  do {
    intersection -= ext.width;
    tui::puts(xorg.x, xorg.y + i + line, str.data());
    line++;
    // strStart = intersection;
  } while (intersection > v.getContentExtent().width);
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
