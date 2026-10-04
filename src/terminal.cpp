#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>
#include <terminal.hpp>
#include <type.hpp>
#include <vector>

namespace tui {
Extent screenExtent = {65 * 16, 65, 16};
Buffer buffer;
std::string displayOutput;
Modes modes;
bool hasInit = false;

// Error return true, no error return false;
// default width = 65, height = 16
// hasInit to ensure true init
bool init(const uint16_t w, const uint16_t h) {
  if (w < 65 || h < 16)
    return true;
  screenExtent = {1, static_cast<uint16_t>(w + 1), h};
  screenExtent.size = screenExtent.height * screenExtent.width;
  buffer.resize(screenExtent);
  hasInit = true;
  return false;
}

void render() {
  std::vector<tui::Cell> &cells = buffer.Cells();
  if (cells.size() < 1)
    return;
  size_t endLineCounter = screenExtent.width - 1;
  for (size_t i = 0; i < screenExtent.size; i++) {
    Cell &v = cells[i];
    if (v.continuation) {
      printf("%lu: continuation: %d\n", i, v.continuation);
      displayOutput.append(" ");
    }
    if (modes.autoSpace && v.glyph.size() < 1) {
      v.glyph = " ";
    }
    if (modes.autoNewLine && (i == endLineCounter)) {
      if (i < screenExtent.size - 1)
        endLineCounter += screenExtent.width;
      v.glyph = "\n";
    }
    displayOutput.append(v.glyph);
  }
}

void show() {
  if (displayOutput.size() < 1)
    return;
  fwrite(displayOutput.data(), 1, displayOutput.size(), stdout);
}

void put(int x, int y, std::string glyph) { buffer.put(x, y, glyph); }
void puts(int x, int y, std::string &str) {
  std::string d(1, 0);
  for (int i = 0; i < str.size(); i++) {
    d[0] = str[i];
    buffer.put(x + i, y, d);
  }
}
void puts(int x, int y, std::string_view str) {
  std::string d(1, 0);
  for (int i = 0; i < str.size(); i++) {
    d[0] = str[i];
    buffer.put(x + i, y, d);
  }
}

namespace basic {
int vec2itoIdx(int x, int y) { return x + y * screenExtent.width; }
} // namespace basic

} // namespace tui
