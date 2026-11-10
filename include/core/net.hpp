#pragma once

#include <cstdint>
#include <sys/types.h>

namespace net {
struct Response {
  const char *string;
  int status = -1;
  uint16_t type;
};

extern Response fetch();
} // namespace net
