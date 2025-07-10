#include "shi/cell.h"
#include "shi/libraries/core/bool.h"
#include "shi/stream.h"

static void bool_write(const struct sh_cell *v, struct sh_stream *out) {
  sh_putc(out, v->as_bool ? 'T' : 'F');
}

const struct sh_type SH_BOOL = {
  .name = "Bool",
  SH_TYPE_DEFAULTS,
  .write = bool_write
};
