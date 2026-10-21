#include <input.hpp>

AppInput appInput;

const bool AppInput::process() {
  const int &ret = tui::Input.process();
  if (ret)
    inputParser();
  return ret;
}
