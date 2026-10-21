#pragma once

#include "terminal.hpp"
#include <cstring>
#include <poll.h>
#include <string>
#include <unistd.h>

namespace tui {
class InputSystem {
protected:
  struct pollfd fds[1];
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
    int ret = poll(fds, 1, 50);

    if (ret > 0) {
      if (fds[0].revents & POLLIN) {
        read(STDIN_FILENO, &terminalInputBuffer, sizeof(terminalInputBuffer));
        return 1;
      }
    }
    return 0;
  }

  void init() {
    fds[0].fd = 0;
    fds[0].events = POLLIN;
  }

public:
  InputSystem() { init(); }

  const bool *const getKeys() { return keys; }
  int getBaseInput() const { return terminalInputBuffer[0]; }
  const char *getTerminalInput() const { return terminalInputBuffer; }

  const int process() {
    const int &ret = getch();
    if (ret) {
      evaluate();
      std::string s("Input: ");
      for (const char v : terminalInputBuffer) {
        s += std::to_string(v);
      }
      puts(0, 16, s);
    }
    return ret;
  }
};

extern InputSystem Input;

} // namespace tui
