#include "geocon_init.h"

#include <atomic>
#include <stdlib.h>
#include <geocon/config.h>

namespace geocon {
bool init() {
  static std::atomic<bool> is_init(false);
  if (is_init) return true;

  if (setenv("MSPCCS_DATA", MSPCCS_DATA_PATH, 0) != 0) return false;

  return (is_init = true);
}
}
