#include "shi/cell.h"
#include "shi/libraries/core.h"
#include "shi/stream.h"
#include "shi/type.h"

static void meta_write(const struct sh_cell *v, struct sh_stream *out) {
  struct sh_type *m = v->as_other;
  sh_puts(out, m->name);
}

struct sh_type *SH_META() {
  static __thread struct sh_type t = {
    .name = "Meta",
    SH_TYPE_DEFAULTS,
    .write = meta_write
  };
  
  return &t;
}
