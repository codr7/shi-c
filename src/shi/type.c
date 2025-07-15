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

struct sh_type *__sh_type_init(struct sh_type *t,
			       const char *name,
			       struct sh_type *parents[]) {
  t->name = strdup(name);
  t->reference_count = 1;
  
  sh_set_init(&t->parents,
	      &sh_malloc_default,
	      sizeof(struct sh_type *),
	      compare_key);

  t->copy = NULL;
  t->deinit = NULL;
  t->emit = NULL;

  while (*parents) {
    struct sh_type *pt = *parents++;
    *(struct sh_type **)sh_set_add(&t->parents, &pt, false) = pt;

    sh_vector_do(&pt->parents.items, ppt) {
      *(struct sh_type **)sh_set_add(&t->parents, ppt, false) =
	*(struct sh_type **)ppt;
    }
  }
  
  return t;
}

struct sh_type *sh_type_acquire(struct sh_type *t) {
  t->reference_count++;
  return t;
}

void sh_type_release(struct sh_type *t) {
  if (!t->reference_count) {
    sh_throw("Type already released: %s", t->name);
  }
  
  if (!--t->reference_count) {
    free(t->name);
    sh_set_deinit(&t->parents);
    free(t);    
  }
}
