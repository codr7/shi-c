#include <inttypes.h>
#include <stdlib.h>

#include "shi/cell.h"
#include "shi/libraries/core.h"
#include "shi/stream.h"
#include "shi/type.h"

static bool as_bool(struct sh_cell *v) {
  return v->as_int;
}

static void dump(const struct sh_cell *v, struct sh_stream *out) {
  sh_printf(out, "%" PRId64, v->as_int);
}

static bool eq(const struct sh_cell *x, const struct sh_cell *y) {
  return x->as_int == y->as_int;
}

struct sh_type *SH_INT() {
  static __thread struct sh_type *t = NULL;

  if (!t) {
    t = malloc(sizeof(struct sh_type));
    sh_type_init(t, "Int", SH_ANY());
    t->as_bool = as_bool;
    t->dump = dump;
    t->eq = eq;
  }

  return t;
}
