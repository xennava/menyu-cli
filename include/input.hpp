#pragma once

#include <cstdint>
namespace tui {
struct InputSystem {
  union {
    uint16_t raw;
    struct {
      uint16_t NONE : 1 = false;
      uint16_t enter : 1;
      uint16_t tab : 1;
      uint16_t alt : 1;
      uint16_t ctrl : 1;
      uint16_t shift : 1;
      uint16_t q : 1;
      uint16_t w : 1;
      uint16_t e : 1;
      uint16_t s : 1;
      uint16_t plus : 1;
      uint16_t minus : 1;
      uint16_t ArrowLeft : 1;
      uint16_t ArrowUp : 1;
      uint16_t ArrowRight : 1;
      uint16_t ArrowDown : 1;
    };
  };
};

extern InputSystem Input;

namespace input {
extern char inputBuffer[16];
extern void init();
extern int getch();
} // namespace input
} // namespace tui

extern void inputParser();
