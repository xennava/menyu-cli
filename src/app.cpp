#include <app.hpp>
#include <cstdint>

bool app::isRunning = false;
bool app::orderPage = false;
uint64_t app::time::dt_msec = 0;
uint64_t app::time::targetFrameTime = 1e5 / 6;
