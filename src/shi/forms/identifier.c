#include "shi/error.h"
#include "shi/evaluate.h"
#include "shi/forms/identifier.h"
#include "shi/library.h"
#include "shi/malloc.h"
#include "shi/stream.h"
#include "shi/vm.h"

static void emit_imp(struct sh_form *_f,
		     struct sh_vm *vm,
		     struct sh_list *args) {
  struct sh_identifier *f = sh_baseof(_f, struct sh_identifier, form);
  struct sh_cell *v = sh_find(vm->library, f->name);
  
  if (!v) {
    sh_throw("Error in %s: Unknown identifier '%s'",
	     sh_sloc_string(&_f->sloc), f->name);
  }

  sh_cell_emit(v, vm, _f->sloc, args);
}

static void dump_imp(const struct sh_form *_f, struct sh_stream *out) {
  struct sh_identifier *f = sh_baseof(_f, struct sh_identifier, form);
  sh_puts(out, f->name);
}

static void free_imp(struct sh_form *_f, struct sh_vm *vm) {
  struct sh_identifier *f = sh_baseof(_f, struct sh_identifier, form);
  sh_release(vm->malloc, f->name);
  sh_release(vm->malloc, f);
}

static struct sh_cell *value_imp(struct sh_form *_f, struct sh_vm *vm) {
  struct sh_identifier *f = sh_baseof(_f, struct sh_identifier, form);  
  return sh_find(vm->library, f->name);
}

const struct sh_form_type SH_IDENTIFIER = {
  .dump = dump_imp,
  .emit = emit_imp,
  .free = free_imp,
  .value = value_imp
};

void sh_identifier_init(struct sh_identifier *f,
			struct sh_vm *vm,
			const struct sh_sloc sloc,
			struct sh_list *owner,
			const char *name) {
  sh_form_init(&f->form, &SH_IDENTIFIER, sloc, owner);
  f->name = sh_strdup(name, vm->malloc);
}
