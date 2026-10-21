#include "appState.hpp"
#include "input.hpp"
#include <core/input.hpp>
#include <core/terminal.hpp>
#include <iostream>
#include <mainMenu.hpp>
#include <ui.hpp>

void orderInput() {
  if (tui::Input.getKeys()[83]) {
    appState.orderPage = false;
  }
}

int main(int argc, char *argv[]) {
  if (tui::init(65, 18)) {
    std::cerr << "error init\n";
  }

  mainMenuInit();

  ui::View orderPage(0, 0, tui::screenExtent.width - 1,
                     tui::screenExtent.height);

  auto searchItem = [&](int i) {
    for (auto &v : productItems)
      if (v.id == order.items[i].menuId)
        return v;
    return MenuItem();
  };

  bool isRunning = true;
  while (isRunning) {
    tui::clearScreen();

    if (appInput.process()) {
      if (appInput.getKey().q) {
        isRunning = false;
      }

      if (!appState.orderPage)
        mainMenuInput();

      if (appInput.getKey().s) {
        appState.orderPage = true;
      }

      orderInput();
    }

    if (appState.orderPage) {
      listProp.margin.left = 3;
      listProp.margin.top = 2;
      listProp.marker.gap = 3;
      listProp.maxElement = order.items.size();
      listProp.marker.selectedElement = 0;
      for (int i = 0; i < order.items.size(); i++)
        ui::list(orderPage, i, searchItem(i).nama, listProp);

      orderPage.render();
    } else {
      mainMenu();
    }

    tui::render();
    tui::show();
  }
#ifndef NDEBUG
  for (const auto &c : ui::view::viewRef) {
    printf("#%d\n", c.second->getID());
  }
#endif
  tui::end();
  return 0;
}
