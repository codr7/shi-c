#include <inttypes.h>

#include "shi/cell.h"
#include "shi/libraries/core/int.h"
#include "shi/stream.h"

static void int_write(const struct sh_cell *v, struct sh_stream *out) {
  sh_printf(out, "%" PRId64, v->as_int);
}

const struct sh_type SH_INT = {
  .name = "Int",
  SH_TYPE_DEFAULTS,
  .write = int_write
};
