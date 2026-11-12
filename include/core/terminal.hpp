#pragma once

#include "termui.hpp"
#include "type.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

namespace tui {
struct Cell {
  std::string glyph;
  uint8_t width;
  bool continuation;
};

struct Modes {
  bool autoNewLine : 1 = true;
  bool autoSpace : 1 = true;
};

extern Modes modes;

extern Extent screenExtent;

extern std::string displayOutput;

namespace basic {
extern int vec2itoIdx(int x, int y);
} // namespace basic

class Buffer {
private:
  std::vector<Cell> cells;

public:
  void put(std::size_t idx, std::string_view glyph) {
    Cell &d = cells[idx];
    d.glyph = glyph;
    int w = termui::display_width(glyph);
    d.width = w;
    while (w > 1) {
      if (++idx >= screenExtent.size)
        return;
      d = cells[idx];
      d.continuation = true;
      w--;
    }
  }
  void put(int x, int y, std::string_view glyph) {
    std::size_t idx = basic::vec2itoIdx(x, y);
    if (idx >= screenExtent.size)
      return;
    Cell &d = cells[idx];
    d.glyph = glyph;
    int w = termui::display_width(glyph);
    d.width = w;
    while (w > 1) {
      if (++idx >= screenExtent.size)
        return;
      d = cells[idx];
      d.continuation = true;
      w--;
    }
  };
  void resize(Extent &ext) { cells.resize(ext.size); }
  Cell *data() { return cells.data(); }
  int size() { return cells.size(); }

  void render(std::string &output) {
    if (cells.size() < 1)
      return;
    for (size_t i = 0; i < screenExtent.size; i++) {
      Cell &v = cells[i];
      if (v.continuation) {
        output.append(" ");
        continue;
      }
      output.append(v.glyph);
    }
  }
  void clear() { cells.assign(cells.size(), Cell{}); };
};

extern Buffer buffer;
extern bool hasInit;
extern bool init(const uint16_t w = 65, const uint16_t h = 16);
extern void moveCursor(int x, int y);
extern void put(int x, int y, std::string_view glyph);
extern void puts(int x, int y, std::string &str);
extern void puts(int x, int y, std::string_view str);
extern void puts(std::string_view str);
extern void render();
extern void show();
extern void clearScreen();
extern void end();

} // namespace tui
