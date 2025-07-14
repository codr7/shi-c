#include "shi/cell.h"
#include "shi/libraries/core.h"
#include "shi/stream.h"
#include "shi/type.h"

static void dump(const struct sh_cell *v, struct sh_stream *out) {
  sh_putc(out, v->as_bool ? 'T' : 'F');
}

static bool eq(const struct sh_cell *x, const struct sh_cell *y) {
  return x->as_bool == y->as_bool;
}

struct sh_type *SH_BOOL() {
  static __thread struct sh_type t = {
    SH_TYPE_DEFAULTS,
    .dump = dump,
    .eq = eq
  };

  return sh_type_init(&t, "Bool");
}
