#include "termui.hpp"
#include <utf8proc.h>

namespace termui {

int display_width(std::string_view text) {
  int width = 0;

  for (std::size_t i = 0; i < text.size();) {
    utf8proc_int32_t codepoint;

    auto n = utf8proc_iterate(
        reinterpret_cast<const utf8proc_uint8_t *>(text.data() + i),
        text.size() - i, &codepoint);

    if (n < 0)
      return -1;

    width += utf8proc_charwidth(codepoint);
    i += static_cast<std::size_t>(n);
  }

  return width;
}

} // namespace termui
