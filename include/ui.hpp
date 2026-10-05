#pragma once

#include "terminal.hpp"
#include "type.hpp"
#include <cstdint>
#include <string>
#include <string_view>

namespace ui {
extern void box(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

enum class Alignment { NONE = 0, CENTERED, LEFT, RIGHT, TOP, BOTTOM, JUSTIFY };

struct Padding {
  int top = 1, right = 1, bottom = 1, left = 1;
  void vertical(const int y) { top = bottom = y; }
  void horizontal(const int x) { left = right = x; }
  void all(const int x) { top = right = bottom = left = x; }
};

using Margin = Padding;

class View {
private:
  struct {
    std::string str;
    Alignment alignment = Alignment::LEFT;
  } title;

  struct {
    Padding padding;

    int x = 0, y = 0;
    int w = 0, h = 0;

  } measurement;

  struct {
    bool borders : 1 = true;
    bool title : 1 = true;
  } modes;

  // uint8_t uid;
  void initContent(vec2i x) { measurement.padding.all(1); }

public:
  struct {
    bool activeView : 1 = false;
  } state;
  View(int x, int y, int w, int h, std::string_view title)
      : measurement({{}, x, y, w, h}) {

    measurement.padding.all(1);
    this->title.str.assign(title);
  }
  View(int x, int y, Extent ext, std::string_view title,
       Padding pad = {0, 0, 0, 0})
      : measurement(pad, x, y, ext.width, ext.height) {

    measurement.padding.all(1);
    this->title.str.assign(title);
  }

  void setTitle(std::string_view title, Alignment aln = Alignment::LEFT) {
    this->title.str.assign(title);
    this->title.alignment = aln;
  }

  void setPadding(int top, int right, int bottom, int left) {
    measurement.padding = {top, right, bottom, left};
  }
  void setPadding(Padding pad) { measurement.padding = pad; }

  Padding getPadding() { return measurement.padding; }
  vec2i getOrigin() { return {measurement.x, measurement.y}; }
  vec2i getContentOrigin() {
    return {measurement.x + measurement.padding.left,
            measurement.y + measurement.padding.top};
  }
  Extent getContentExtent() {
    return {.width = static_cast<int>(measurement.w - getContentOrigin().x -
                                      measurement.padding.right),
            .height = static_cast<int>(measurement.h - getContentOrigin().y -
                                       measurement.padding.bottom)};
  }
  void render() {
    box(measurement.x, measurement.y, measurement.w, measurement.h);
    if (modes.title) {
      tui::puts(measurement.x + 1, measurement.y, title.str);
    }
  }
};

struct ListProperty {
  struct {
    std::string_view glyph;
    uint8_t gap = 1;
    uint8_t selectedElement = 0;
  } marker;
  struct {
  private:
    bool clipTxt : 1 = 1;
    bool wrapTxt : 1 = 0;

  public:
    bool autoMarkAll : 1 = 0;
    bool autoMarkSelected : 1 = 1;
    void wrapText() {
      wrapTxt = 1;
      clipTxt = 0;
    }
    void clipText() {
      clipTxt = 1;
      wrapTxt = 0;
    }
  } modes;
  Margin margin = {0, 0, 0, 0};
  uint8_t uid;
};

// modes:
//  0x0 : plain
//  0x1 : With Auto Marker to all element
//  0x2 : Auto marker to selected element
extern void list(View &v, int &i, std::string_view str, uint8_t selectedElement,
                 uint8_t modes = 0x2, std::string_view marker = ">");
extern void list(View &v, int &i, std::string_view str, ListProperty &lp);
extern void header(std::string_view str);
extern void footer(std::string_view str);
} // namespace ui
