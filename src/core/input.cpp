#include <core/input.hpp>

namespace tui {
InputSystem Input;

void InputSystem::evaluate() {
  memset(keys, 0, sizeof(keys));
  for (const auto i : terminalInputBuffer) {
    if (i > 0)
      keys[(int)i] = true;
  }
}
int InputSystem::getch() {
#ifdef _WIN32
  if (!_kbhit()) {
    return 0;
  }
  memset(terminalInputBuffer, 0, sizeof(terminalInputBuffer));

  const int ch = _getch();

  terminalInputBuffer[0] = ch;
  if (ch == 224) {
    terminalInputBuffer[1] = _getch();
  }
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
} // namespace tui
