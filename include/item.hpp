#pragma once

#include "order.hpp"
#include "pages.hpp"

extern std::vector<MenuItem> productItems;

inline MenuItem *searchItemByCategory(int category) {
  for (auto &v : productItems) {
    if (v.category == category + 1) {
      return &v;
    }
  }
  return nullptr;
};
