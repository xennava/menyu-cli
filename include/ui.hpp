#pragma once

#include "core/type.hpp"
#include <core/terminal.hpp>
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

class View;

namespace view {

extern const View *getActiveView();
extern std::unordered_map<int, View *> viewRef;
extern void makeUniqueActiveView();
extern void toggleActiveView();
extern void stepActiveView(int bipolar);

} // namespace view

class View {
protected:
  bool hasInit = false;

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

  uint8_t uid = 0;
  void initContent(vec2i x) { measurement.padding.all(1); }

public:
  struct {
    bool activeView : 1 = false;
  } state;

  View(int x, int y, int w, int h, std::string_view legend = "")
      : measurement({{}, x, y, w, h}) {

    if (legend.empty()) {
      modes.title = 0;
      return;
    }
    this->title.str.assign(legend);
  }

  View(int x, int y, Extent ext, std::string_view legend,
       Padding pad = {0, 0, 0, 0})
      : measurement(pad, x, y, ext.width, ext.height) {

    if (legend.empty()) {
      modes.title = 0;
      return;
    }
    this->title.str.assign(legend);
  }

  ~View() {}
  void init();
  void makeDefaultActiveView();

  void setTitle(std::string_view title, Alignment aln = Alignment::LEFT) {
    this->title.str.assign(title);
    this->title.alignment = aln;
  }

  void setPadding(int top, int right, int bottom, int left) {
    measurement.padding = {top, right, bottom, left};
  }

  void setPadding(Padding pad) { measurement.padding = pad; }

  const Padding &getPadding() { return measurement.padding; }
  const vec2i getOrigin() { return {measurement.x, measurement.y}; }
  const vec2i getContentOrigin() {
    return {measurement.x + measurement.padding.left,
            measurement.y + measurement.padding.top};
  }
  const Extent getContentExtent() {
    return {
        .width = static_cast<int>(measurement.w - measurement.padding.right),
        .height = static_cast<int>(measurement.h - measurement.padding.bottom)};
  }
  void render() {
    box(measurement.x, measurement.y, measurement.w, measurement.h);
    if (modes.title) {
      tui::puts(measurement.x + 1, measurement.y, title.str);
    }
  }
  const int getID() { return uid; }
};

class Text {
protected:
  bool isWrap = 0;
  vec2i originPos;
  vec2i contentPos;
  Extent viewContentExtent;
  Padding padding = {0, 0, 0, 0};
  bool centered = false;
  vec2i calculatedPos;

private:
  void wrapText(std::string &s) {
    int len = s.size();
    int line = 0;
    int startIdx = 0;
    while (len > viewContentExtent.width) {
      len -= viewContentExtent.width;
      tui::puts(contentPos.x, contentPos.y + line,
                s.substr(startIdx, viewContentExtent.width));
      line++;
      if (line > viewContentExtent.height)
        return;
      startIdx += viewContentExtent.width - 1;
    }
    if (line > viewContentExtent.height)
      return;
    tui::puts(contentPos.x, contentPos.y + line,
              s.substr(startIdx, s.size() - startIdx));
  }

  std::string str;

public:
  Text(View &v) {
    originPos = v.getContentOrigin();
    contentPos = originPos;
    viewContentExtent = v.getContentExtent();
  }
  Text(View &v, Padding p) {
    originPos = v.getContentOrigin();
    contentPos = {.x = originPos.x + p.left, .y = originPos.y - p.bottom};
    padding = p;
    viewContentExtent = v.getContentExtent();
  }

  void setClipToText() { isWrap = false; }

  const vec2i &getOriginPos() { return originPos; }
  Extent getExtent() {
    Extent e;
    e.height = 1;
    e.width = viewContentExtent.width;
    return e;
  }
  Text &setPadding(Padding p) {
    padding = p;
    return *this;
  }

  Text &center() {
    if (!centered) {
      contentPos.x =
          originPos.x + ((viewContentExtent.width - str.size()) >> 1);

      centered = true;
    }
    return *this;
  }
  Text &add(std::string_view s) {
    str += s;
    return *this;
  }

  Text &assign(std::string_view s) {
    if (str.compare(s)) {
      str.assign(s);
      centered = 0;
    }
    return *this;
  }

  Text &setOriginPos(vec2i pos) {
    originPos = pos;
    contentPos = pos;
    calculatedPos = pos;
    return *this;
  }

  void render() {
    if (isWrap) {
      wrapText(str);
      return;
    }

    contentPos.y = originPos.y + padding.top;
    // contentPos.x = calculatedPos.x + padding.left;

    int n = viewContentExtent.width;
    if (str.size() > n) {
      n -= 3 + padding.right;
      tui::puts(contentPos.x + n - 1, contentPos.y, "...");
    }
    tui::puts(contentPos.x + padding.left, contentPos.y, str.substr(0, n));
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
  uint16_t maxElement = 0;
};

extern void lineSeparator(View &v, Text &t);
extern void lineSeparator(vec2i orgPosRef, Extent extRef, int width);

// modes:
//  0x0 : plain
//  0x1 : With Auto Marker to all element
//  0x2 : Auto marker to selected element
extern void list(View &v, int &i, std::string_view str, ListProperty &lp);
extern void header(std::string_view str);
extern void footer(std::string_view str);
} // namespace ui
