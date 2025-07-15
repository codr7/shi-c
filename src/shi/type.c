#include <stdlib.h>
#include <string.h>

#include "shi/cell.h"
#include "shi/error.h"
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

static void default_copy(struct sh_cell *dst, struct sh_cell *src,
			 struct sh_vm *vm) {
  *dst = *src;
}

static void default_emit(struct sh_cell *v,
			 struct sh_vm *vm,
			 struct sh_sloc sloc,
			 struct sh_list *args) {
  struct sh_push_value op;
  sh_cell_copy(&op.value, v, vm);
  sh_emit(vm, &SH_PUSH_VALUE, &op);
}

struct sh_type *__sh_type_init(struct sh_type *t,
			       const char *name,
			       struct sh_type *parents[]) {
  t->name = strdup(name);
  t->reference_count = 0;
  
  sh_set_init(&t->parents,
	      &sh_malloc_default,
	      sizeof(struct sh_type *),
	      compare_key);

  t->copy = default_copy;
  t->emit = default_emit;

  while (*parents) {
    struct sh_type *pt = *parents;
    *(struct sh_type **)sh_set_add(&t->parents, &pt, false) = pt;

    sh_vector_do(&pt->parents.items, ppt) {
      *(struct sh_type **)sh_set_add(&t->parents, ppt, false) =
	*(struct sh_type **)ppt;
    }

    parents++;
  }
  
  return t;
}

struct sh_type *sh_type_acquire(struct sh_type *t) {
  t->reference_count++;
  return t;
}

void sh_type_release(struct sh_type *t) {
  if (!t->reference_count) {
    sh_throw("Type already released");
  }
  
  if (!--t->reference_count) {
    free(t->name);
    sh_set_deinit(&t->parents);
    free(t);    
  }
}
