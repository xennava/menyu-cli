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
  void evaluate();
  int getch();

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
