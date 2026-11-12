#include <input.hpp>

AppInput appInput;

bool AppInput::process() {
  const int &ret = tui::Input.process();
  if (ret)
    inputParser();
  return ret;
}
