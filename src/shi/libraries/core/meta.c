#include "shi/cell.h"
#include "shi/libraries/core.h"
#include "shi/stream.h"
#include "shi/type.h"

static void dump(const struct sh_cell *v, struct sh_stream *out) {
  struct sh_type *m = v->as_other;
  sh_puts(out, m->name);
}

static bool eq(const struct sh_cell *x, const struct sh_cell *y) {
  return x->as_other == y->as_other;
}

struct sh_type *SH_META() {
  static __thread struct sh_type t = {
    SH_TYPE_DEFAULTS,
    .dump = dump,
    .eq = eq
  };
  
  return sh_type_init(&t, "Meta");
}
