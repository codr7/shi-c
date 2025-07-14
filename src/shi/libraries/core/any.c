#include "shi/libraries/core.h"
#include "shi/type.h"

struct sh_type *SH_ANY() {
  static __thread struct sh_type t = {
    .name = "Any",
    SH_TYPE_DEFAULTS
  };
  
  return &t;
}
