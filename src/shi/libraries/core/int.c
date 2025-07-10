#include <inttypes.h>

#include "shi/cell.h"
#include "shi/libraries/core.h"
#include "shi/stream.h"
#include "shi/type.h"

static void int_write(const struct sh_cell *v, struct sh_stream *out) {
  sh_printf(out, "%" PRId64, v->as_int);
}

const struct sh_type *SH_INT() {
  static __thread const struct sh_type t = {
    .name = "Int",
    SH_TYPE_DEFAULTS,
    .write = int_write
  };
  
  return &t;
}
