#pragma once
#include <cstdint>
#include <functional>

struct vec2ui16 {
  uint16_t x, y;
};
struct Extent {
  uint32_t size = 0;
  int width, height;
};
struct vec2i {
  int x, y;
};
template <typename T> class Counter {
private:
  T ctr = 0;

public:
  void increment() { ctr++; }
  void decrement() { ctr--; }
  void step(T step) { ctr += step; }
  void zero() { ctr = 0; }
  bool isEqualTo(T x) {
    if (ctr == x)
      return true;
    return false;
  }
  void conditional(std::function<int(int)> &fun) { fun(ctr); }
  void conditional(std::function<int(int)> fun) { fun(ctr); }
  const T &getCtr() { return ctr; }
};
