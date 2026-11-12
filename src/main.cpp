#include "app.hpp"
#include "fileLock.hpp"
#include "input.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <pages.hpp>
#include <ratio>

int main() {
  FileLock fl;
  if (!fl.acquire()) {
    return 1;
  }

  if (tui::init(65, 18)) {
    std::cerr << "error init\n";
  }

  pagesInit();

  bool isRunning = true;
  auto timeNow = std::chrono::steady_clock::now();

  uint64_t accumulator = 0;

  while (isRunning) {
    auto timeLast = std::chrono::steady_clock::now();
    auto microDuration =
        std::chrono::duration_cast<std::chrono::duration<uint64_t, std::micro>>(
            timeLast - timeNow);
    app::time::dt_msec = microDuration.count();
    timeNow = timeLast;

    accumulator += app::time::dt_msec;

    tui::clearScreen();

    if (appInput.process()) {
      if (appInput.getKey().q) {
        isRunning = false;
      }

      pagesInput();
    }

    if (accumulator >= app::time::targetFrameTime) {
      pagesUpdate();
      accumulator -= app::time::targetFrameTime;
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
