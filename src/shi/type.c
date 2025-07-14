#include <stdlib.h>
#include <string.h>

#include "shi/cell.h"
#include "shi/evaluate.h"
#include "shi/malloc.h"
#include "shi/method.h"
#include "shi/sloc.h"
#include "shi/type.h"
#include "shi/stream.h"
#include "shi/vm.h"

static enum sh_order compare_key(const void *x, const void *y) {
  return sh_compare(*(struct sh_type **)x, *(struct sh_type **)y); 
}

struct sh_type *sh_type_init(struct sh_type *t, const char *name) {
  t->name = strdup(name);
  
  sh_set_init(&t->parents,
	      &sh_malloc_default,
	      sizeof(struct sh_type *),
	      compare_key);

  return t;
}

void sh_type_deinit(struct sh_type *t) {
  free(t->name);
  sh_set_deinit(&t->parents);
}

void sh_type_default_copy(struct sh_cell *dst, struct sh_cell *src,
			 struct sh_vm *vm) {
  *dst = *src;
}

void sh_type_default_emit(struct sh_cell *v,
			 struct sh_vm *vm,
			 struct sh_sloc sloc,
			 struct sh_list *args) {
  struct sh_push_value op;
  sh_cell_copy(&op.value, v, vm);
  sh_emit(vm, &SH_PUSH_VALUE, &op);
}
