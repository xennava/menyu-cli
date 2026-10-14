#include "terminal.hpp"
#include <input.hpp>
#include <poll.h>
#include <string.h>
#include <string>
#include <unistd.h>

namespace tui {
InputSystem Input;
namespace input {
struct pollfd fds[1];
char inputBuffer[16];
void init() {
  fds[0].fd = 0;
  fds[0].events = POLLIN;
}
int getch() {
  int ret = poll(fds, 1, 50);

  if (ret > 0) {
    if (fds[0].revents & POLLIN) {
      read(STDIN_FILENO, &inputBuffer, sizeof(inputBuffer));
      return 0;
    }
  }
  return -1;
}
} // namespace input
} // namespace tui

void inputParser() {
  using tui::Input;
  using tui::input::inputBuffer;

  if (inputBuffer[0] == 0x1b) {
    Input.alt = true;
    if (inputBuffer[1] > 96 && inputBuffer[1] < 123) {
    }
  }
  if (inputBuffer[0] < 27 && inputBuffer[0] > 0) {
    Input.ctrl = true;
  }

  std::string s = "Input: ";
  for (int i = 0; i < 16; i++)
    s += std::to_string(inputBuffer[i]);
  tui::puts(0, 16, s);
  std::string v = "State: ";
  if (Input.alt)
    v += "alt";
  if (Input.ctrl)
    v += "ctrl";
  tui::puts(0, 17, v);

  memset(inputBuffer, 0, sizeof(inputBuffer));
  Input.raw = 0;
}
