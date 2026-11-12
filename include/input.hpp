#pragma once

#include <core/input.hpp>
#include <cstdint>

class AppInput {
protected:
  const bool *keys = nullptr;
  struct Type {
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
  union Key {
    uint16_t raw = 0;
    Type type;
  } key;

  void alphabetParse() {
    if (keys['q'] || keys['Q'])
      key.type.q = 1;
    if (keys['w'] || keys['W'])
      key.type.w = 1;
    if (keys['e'] || keys['E'])
      key.type.e = 1;
    if (keys['s'] || keys['S'])
      key.type.s = 1;
  };

private:
  void inputParser() {
    if (keys == nullptr || keys == NULL)
      return;

    key.raw = 0;

    int c = tui::Input.getBaseInput();
    if (c >= 'A' && c <= 'Z')
      key.type.shift = 1;
#ifdef _WIN32
    if (keys[72])
      key.type.ArrowUp = true;
    if (keys[80])
      key.type.ArrowDown = true;
    if (keys[75])
      key.type.ArrowLeft = true;
    if (keys[77])
      key.type.ArrowRight = true;
    if (keys[13])
      key.type.enter = true;
#else
    if (c == 27) {
      key.type.alt = true;
      if (keys[91]) {
        if (keys[65])
          key.type.ArrowUp = true;
        if (keys[66])
          key.type.ArrowDown = true;
        if (keys[67])
          key.type.ArrowRight = true;
        if (keys[68])
          key.type.ArrowLeft = true;
      }
    }
    if (c == 61)
      key.type.plus = true;
    if (c == 45)
      key.type.minus = true;
    if (c == 9)
      key.type.tab = true;
    if (c == 10)
      key.type.enter = true;
#endif
    alphabetParse();
  }

public:
  AppInput() { keys = tui::Input.getKeys(); }

  bool process();
  const Type &getKey() { return key.type; }
};

extern AppInput appInput;
