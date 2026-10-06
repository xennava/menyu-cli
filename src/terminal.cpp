#include <cstdint>
#include <cstdio>
#include <poll.h>
#include <string>
#include <string_view>
#include <terminal.hpp>
#include <termios.h>
#include <type.hpp>
#include <unistd.h>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace tui {
Extent screenExtent = {65 * 16, 65, 16};
Buffer buffer;
std::string displayOutput;
Modes modes;
bool hasInit = false;
struct pollfd fds[1];

int getch() {
  int ret = poll(fds, 1, 100);

  if (ret > 0) {
    // poll mendeteksi ada input yang siap dibaca!
    if (fds[0].revents & POLLIN) {
      char ch;
      read(0, &ch, 1);
      return ch;
    }
  }
  return -1;
}
void setTerminalMode(int enableRaw) {
#ifdef _WIN32
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  GetConsoleCursorInfo(hOut, &cursorInfo);
  cursorInfo.bVisible = visible;
  SetConsoleCursorInfo(hOut, &cursorInfo);
#else
  struct termios t;
  tcgetattr(0, &t);

  if (enableRaw) {
    t.c_lflag &= ~(ICANON | ECHO);
    t.c_cc[VMIN] = 0;
    t.c_cc[VTIME] = 0;
    printf("\033[?25l");
  } else {
    t.c_lflag |= (ICANON | ECHO);
    t.c_cc[VMIN] = 1;
    t.c_cc[VTIME] = 0;
    printf("\033[?25h");
  }

  tcsetattr(0, TCSANOW, &t);
#endif
}

// Error return true, no error return false;
// default width = 65, height = 16
// hasInit to ensure true init
bool init(const uint16_t w, const uint16_t h) {
  if (w < 65 || h < 16)
    return true;

  fds[0].fd = 0;
  fds[0].events = POLLIN;

  screenExtent = {1, static_cast<uint16_t>(w + 1), h};
  screenExtent.size = screenExtent.height * screenExtent.width;
  buffer.resize(screenExtent);
  setTerminalMode(1);
  printf("\033[2J");
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

void clearScreen() {
  printf("\033[H");
  buffer.clear();
  displayOutput.clear();
  displayOutput.shrink_to_fit();
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

void puts_n(int x, int y, int n, std::string_view str) {
  if (n > str.size() - 1)
    return;
  std::string d(1, 0);

  for (int i = 0; i < n; i++) {
    d[0] = str[i];
    buffer.put(x + i, y, d);
  }
}

void end() { setTerminalMode(0); }

namespace basic {
int vec2itoIdx(int x, int y) { return x + y * screenExtent.width; }
} // namespace basic

} // namespace tui
