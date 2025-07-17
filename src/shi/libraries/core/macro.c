#include <stdlib.h>

#include "shi/cell.h"
#include "shi/form.h"
#include "shi/libraries/core.h"
#include "shi/list.h"
#include "shi/macro.h"
#include "shi/sloc.h"
#include "shi/stream.h"
#include "shi/type.h"
#include "shi/vm.h"

static void copy(struct sh_cell *dst, struct sh_cell *src, struct sh_vm *vm) {
  dst->as_other = sh_macro_acquire(src->as_other);
}

static void deinit(struct sh_cell *v) {
  sh_macro_release(v->as_other);
}

static void dump(const struct sh_cell *v, struct sh_stream *out) {
  struct sh_macro *m = v->as_other;
  sh_printf(out, "Macro %s", m->name);
}

static void emit(struct sh_cell *v,
		 struct sh_vm *vm,
		 struct sh_sloc sloc,
		 struct sh_list *arguments) {
  sh_macro_emit(v->as_other, &sloc, arguments);
}

static bool eq(const struct sh_cell *x, const struct sh_cell *y) {
  return x->as_other == y->as_other;
}

struct sh_type *SH_MACRO() {
  static __thread struct sh_type *t = NULL;

  if (!t) {
    t = malloc(sizeof(struct sh_type));
    sh_type_init(t, "Macro", SH_ANY());
    t->copy = copy;
    t->deinit = deinit;
    t->dump = dump;
    t->emit = emit;
    t->eq = eq;
  }

  return t;
}
