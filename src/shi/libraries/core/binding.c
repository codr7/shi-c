#include <stdlib.h>

#include "shi/cell.h"
#include "shi/form.h"
#include "shi/libraries/core.h"
#include "shi/list.h"
#include "shi/operation.h"
#include "shi/sloc.h"
#include "shi/stream.h"
#include "shi/type.h"
#include "shi/vm.h"

static void copy(struct sh_cell *dst, struct sh_cell *src, struct sh_vm *vm) {
  dst->as_register = src->as_register;
}

static void dump(const struct sh_cell *v, struct sh_stream *out) {
  sh_printf(out, "#%ld", v->as_register);
}

static void emit(struct sh_cell *v,
		 struct sh_vm *vm,
		 struct sh_sloc sloc,
		 struct sh_list *arguments) {
  sh_emit_get_registers(vm, v->as_register, 1);
}

static bool eq(const struct sh_cell *x, const struct sh_cell *y) {
  return x->as_register == y->as_register;
}

struct sh_type *SH_BINDING() {
  static __thread struct sh_type *t = NULL;

  if (!t) {
    t = malloc(sizeof(struct sh_type));
    sh_type_init(t, "Binding");
    t->copy = copy;
    t->dump = dump;
    t->emit = emit;
    t->eq = eq;
  }

  return t;
}
