#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "shi/cell.h"
#include "shi/sloc.h"
#include "shi/stream.h"
#include "shi/type.h"

struct sh_cell *sh_cell_init(struct sh_cell *v, struct sh_type *t) {
  v->type = t;
  return v;
}

void sh_cell_deinit(struct sh_cell *v) {
  if (v->type->deinit) {
    v->type->deinit(v);
  }
}

struct sh_cell *sh_cell_copy(struct sh_cell *dst, struct sh_cell *src,
			     struct sh_vm *vm) {
  struct sh_type *t = src->type;
  assert(t->copy);
  dst->type = t;
  t->copy(dst, src, vm);
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
  assert(v->type->emit);
  v->type->emit(v, vm, sloc, args);
}
