#include <stdlib.h>
#include <string.h>

#include "shi/cell.h"
#include "shi/error.h"
#include "shi/malloc.h"
#include "shi/method.h"
#include "shi/sloc.h"
#include "shi/type.h"
#include "shi/stream.h"
#include "shi/vm.h"

struct sh_type *sh_type_init(struct sh_type *t, const char *name) {
  t->name = strdup(name);
  t->reference_count = 0;
  t->as_bool = NULL;
  t->copy = NULL;
  t->deinit = NULL;
  t->emit = NULL;
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
