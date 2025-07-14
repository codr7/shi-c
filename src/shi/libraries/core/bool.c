#include "shi/cell.h"
#include "shi/libraries/core.h"
#include "shi/stream.h"
#include "shi/type.h"

static void bool_dump(const struct sh_cell *v, struct sh_stream *out) {
  sh_putc(out, v->as_bool ? 'T' : 'F');
}

struct sh_type *SH_BOOL() {
  static __thread struct sh_type t = {
    .name = "Bool",
    SH_TYPE_DEFAULTS,
    .dump = bool_dump
  };
  
  return &t;
}
