#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "shi/cell.h"
#include "shi/operations/push_value.h"
#include "shi/sloc.h"
#include "shi/stream.h"
#include "shi/type.h"
#include "shi/vm.h"

struct sh_cell *sh_cell_init(struct sh_cell *v, struct sh_type *t) {
  v->type = sh_type_acquire(t);
  return v;
}

void sh_cell_deinit(struct sh_cell *v) {
  if (v->type->deinit) {
    v->type->deinit(v);
  }

  sh_type_release(v->type);
}

struct sh_cell *sh_cell_copy(struct sh_cell *dst, struct sh_cell *src,
			     struct sh_vm *vm) {
  struct sh_type *t = sh_type_acquire(src->type);

  if (t->copy) {
    assert(t->copy);
    dst->type = t;
    t->copy(dst, src, vm);
  } else {
    *dst = *src;
  }
  
  return dst;
}

void sh_cell_dump(const struct sh_cell *v, struct sh_stream *out) {
  assert(v->type->dump);
  v->type->dump(v, out);
}

void sh_cell_emit(struct sh_cell *v,
		  struct sh_vm *vm,
		  struct sh_sloc sloc,
		  struct sh_list *args) {
  if (v->type->emit) {
    v->type->emit(v, vm, sloc, args);
  } else {
    struct sh_push_value op;
    sh_cell_copy(&op.value, v, vm);
    sh_emit(vm, &SH_PUSH_VALUE, &op);
  }
}

bool sh_eq(const struct sh_cell *x, const struct sh_cell *y) {
  return x->type == y->type && x->type->eq(x, y);
}

bool sh_isa(const struct sh_cell *v, const struct sh_type *t) {
  return v->type == t || sh_set_find(&v->type->parents, t);
}
