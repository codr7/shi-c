#include <stdlib.h>

#include "shi/cell.h"
#include "shi/libraries/core.h"
#include "shi/stream.h"
#include "shi/type.h"

static void copy(struct sh_cell *dst, struct sh_cell *src, struct sh_vm *vm) {
  dst->as_other = sh_type_acquire(src->as_other);
}

static void deinit(struct sh_cell *v) {
  sh_type_release(v->as_other);
}

static void dump(const struct sh_cell *v, struct sh_stream *out) {
  struct sh_type *m = v->as_other;
  sh_puts(out, m->name);
}

static bool eq(const struct sh_cell *x, const struct sh_cell *y) {
  return x->as_other == y->as_other;
}

struct sh_type *SH_META() {
  static __thread struct sh_type *t = NULL;

  if (!t) {
    t = malloc(sizeof(struct sh_type));
    sh_type_init(t, "Meta");
    t->copy = copy;
    t->deinit = deinit;
    t->dump = dump;
    t->eq = eq;
  }

  return t;
}
