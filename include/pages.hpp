#pragma once

#include "order.hpp"
#include "ui.hpp"

class Pages {
private:
  int activePageIndex = 0;
  int pageCount = 0;

public:
  void init() {}
  void switchPage(int bipolar = 1) {
    if (pageCount == 0)
      return;

    activePageIndex += bipolar;

    if (activePageIndex > pageCount)
      activePageIndex = pageCount - 1;
    if (activePageIndex < 0)
      activePageIndex = 0;
  }
};

extern void pagesInit();
extern void pagesInput();

extern void pagesUpdate();

extern ui::ListProperty listProp;

extern std::vector<MenuItem> productItems;
