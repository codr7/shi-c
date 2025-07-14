#include <inttypes.h>

#include "shi/cell.h"
#include "shi/libraries/core.h"
#include "shi/stream.h"
#include "shi/type.h"

static void dump(const struct sh_cell *v, struct sh_stream *out) {
  sh_printf(out, "%" PRId64, v->as_int);
}

static bool eq(const struct sh_cell *x, const struct sh_cell *y) {
  return x->as_int == y->as_int;
}

struct sh_type *SH_INT() {
  static __thread struct sh_type t = {
    .name = "Int",
    SH_TYPE_DEFAULTS,
    .dump = dump,
    .eq = eq
  };
  
  return &t;
}
