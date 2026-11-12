#pragma once

#include <cstring>
#ifdef _WIN32
#include <conio.h>
#else
#include <poll.h>
#endif
#include <unistd.h>

namespace tui {
class InputSystem {
protected:
#ifndef _WIN32
  struct pollfd fds[1];
#endif // !_WIN32
  bool keys[256];
  char terminalInputBuffer[16];

private:
  void evaluate() {
    memset(keys, 0, sizeof(keys));
    for (const auto i : terminalInputBuffer) {
      if (i > 0)
        keys[(int)i] = true;
    }
  }
  int getch() {
#ifdef _WIN32
    if (!_kbhit()) {
      return 0;
    }

    const int ch = _getch();
    terminalInputBuffer[0] = ch;
    if (ch == 0 || ch == 0xE0)
      terminalInputBuffer[1] = _getch();

    return 1;
#else
    int ret = poll(fds, 1, 50);

    if (ret > 0) {
      if (fds[0].revents & POLLIN) {
        read(STDIN_FILENO, &terminalInputBuffer, sizeof(terminalInputBuffer));
        return 1;
      }
    }
    return 0;
#endif
  }

  void init() {
#ifndef _WIN32
    fds[0].fd = 0;
    fds[0].events = POLLIN;
#endif // !_WIN32
  }

public:
  InputSystem() { init(); }

  const bool *getKeys() { return keys; }
  int getBaseInput() const { return terminalInputBuffer[0]; }
  const char *getTerminalInput() const { return terminalInputBuffer; }

  int process() {
    const int &ret = getch();
    if (ret) {
      evaluate();
    }
    return ret;
  }
};

extern InputSystem Input;

} // namespace tui
