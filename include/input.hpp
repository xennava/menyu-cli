#pragma once

#include <core/input.hpp>
#include <cstdint>

class AppInput {
protected:
  const bool *keys = nullptr;
  union Key {
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
  } key;

  void alphabetParse() {
    if (keys['q'] || keys['Q'])
      key.q = 1;
    if (keys['w'] || keys['W'])
      key.w = 1;
    if (keys['e'] || keys['E'])
      key.e = 1;
    if (keys['s'] || keys['S'])
      key.s = 1;
  };

private:
  void inputParser() {
    if (keys == nullptr || keys == NULL)
      return;

    key.raw = 0;

    int c = tui::Input.getBaseInput();
    if (c >= 'A' && c <= 'Z')
      key.shift = 1;
#ifdef _WIN32
    if (c == 0 || c == 224) {
      if (keys[72])
        key.ArrowUp = true;
      if (keys[80])
        key.ArrowDown = true;
      if (keys[75])
        key.ArrowLeft = true;
      if (keys[77])
        key.ArrowRight = true;
    }
#else
    if (c == 27) {
      key.alt = true;
      if (keys[91]) {
        if (keys[65])
          key.ArrowUp = true;
        if (keys[66])
          key.ArrowDown = true;
        if (keys[67])
          key.ArrowRight = true;
        if (keys[68])
          key.ArrowLeft = true;
      }
    }
    if (c == 61)
      key.plus = true;
    if (c == 45)
      key.minus = true;
    if (c == 9)
      key.tab = true;
    if (c == 10)
      key.enter = true;
#endif
    alphabetParse();
  }

public:
  AppInput() { keys = tui::Input.getKeys(); }

  bool process();
  const Key &getKey() { return key; }
};

extern AppInput appInput;
