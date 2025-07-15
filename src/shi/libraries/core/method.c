#include <stdlib.h>

#include "shi/cell.h"
#include "shi/evaluate.h"
#include "shi/form.h"
#include "shi/libraries/core.h"
#include "shi/list.h"
#include "shi/method.h"
#include "shi/sloc.h"
#include "shi/stream.h"
#include "shi/type.h"
#include "shi/vm.h"

static void copy(struct sh_cell *dst, struct sh_cell *src, struct sh_vm *vm) {
  dst->as_other = sh_method_acquire(src->as_other);
}

static void deinit(struct sh_cell *v) {
  sh_method_release(v->as_other);
}

static void dump(const struct sh_cell *v, struct sh_stream *out) {
  struct sh_method *m = v->as_other;
  sh_printf(out, "Method %s", m->name);
}

static void emit(struct sh_cell *v,
		 struct sh_vm *vm,
		 struct sh_sloc sloc,
		 struct sh_list *arguments) {
  struct sh_method *m = v->as_other;
  
  for (int i = 0; i < m->arity; i++) {
    struct sh_form *f = sh_baseof(sh_list_pop_front(arguments),
				  struct sh_form,
				  owner);

    sh_form_emit(f, vm, arguments);
    sh_form_release(f, vm);
  }

  struct sh_call_method op;
  op.sloc = sloc;
  op.target = m;
  sh_emit(vm, &SH_CALL_METHOD, &op);
}

static bool eq(const struct sh_cell *x, const struct sh_cell *y) {
  return x->as_other == y->as_other;
}

struct sh_type *SH_METHOD() {
  static __thread struct sh_type *t = NULL;

  if (!t) {
    t = malloc(sizeof(struct sh_type));
    sh_type_init(t, "Metod", SH_ANY());
    t->copy = copy;
    t->deinit = deinit;
    t->dump = dump;
    t->emit = emit;
    t->eq = eq;
  }

  return t;
}
