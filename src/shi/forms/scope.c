#include "shi/error.h"
#include "shi/forms/scope.h"
#include "shi/library.h"
#include "shi/malloc.h"
#include "shi/operations/push_value.h"
#include "shi/stream.h"
#include "shi/vm.h"

static void emit_imp(struct sh_form *_f,
		     struct sh_vm *vm,
		     struct sh_list *args) {
  struct sh_scope *f = sh_baseof(_f, struct sh_scope, form);
  struct sh_list as;
  sh_list_init(&as);

  struct sh_vector backup;
  sh_vector_init(&backup, vm->malloc, sizeof(struct sh_list *));
  sh_defer(sh_vector_deinit(&backup));
  
  sh_list_do(&f->items, i) {
    *(struct sh_list **)sh_vector_push(&backup) = i;
  }

  sh_forms_emit(&f->items, vm);

  sh_vector_do(&backup, i) {
    sh_list_push_back(&f->items, *(struct sh_list **)i);
  }
}

static void dump_imp(const struct sh_form *_f, struct sh_stream *out) {
  struct sh_scope *f = sh_baseof(_f, struct sh_scope, form);

  sh_list_do(&f->items, i) {
    sh_form_dump(sh_baseof(i, struct sh_form, owner), out);
  }
}

static void free_imp(struct sh_form *_f, struct sh_vm *vm) {
  struct sh_scope *f = sh_baseof(_f, struct sh_scope, form);  

  sh_list_do(&f->items, i) {
    sh_form_release(sh_baseof(i, struct sh_form, owner), vm);
  }
}

static struct sh_cell *value_imp(struct sh_form *_f, struct sh_vm *vm) {
  return NULL;
}

const struct sh_form_type SH_SCOPE = {
  .dump = dump_imp,
  .emit = emit_imp,
  .free = free_imp,
  .value = value_imp
};

void sh_scope_init(struct sh_scope *f,
		     struct sh_vm *vm,
		     struct sh_sloc sloc,
		     struct sh_list *owner) {
  sh_form_init(&f->form, &SH_SCOPE, sloc, owner);
}
