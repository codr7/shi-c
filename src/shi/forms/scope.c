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

  sh_library_do(vm) {
    sh_forms_emit(&f->items, vm);
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
  sh_list_init(&f->items);
}
