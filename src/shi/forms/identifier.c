#include "shi/error.h"
#include "shi/forms/identifier.h"
#include "shi/malloc.h"
#include "shi/stream.h"
#include "shi/vm.h"

static void identifier_emit(struct sh_form *_f, struct sh_vm *vm) {
  /*
    struct sh_identifier *f = sh_baseof(_f, struct sh_identifier, form);
    struct sh_value *v = sh_vm_getenv(vm, f->name);

    if (!v) {
    sh_throw("Error in %s: Unknown identifier '%s'",
    sh_sloc_string(&_f->sloc), f->name);
    }

    struct sh_push_op op;
    sh_value_copy(&op.value, v);
    sh_vm_emit(&vm->vm, &SH_PUSH, &op);
  */

  sh_throw("Not implemented!");
}

static void identifier_dump(const struct sh_form *_f, struct sh_stream *out) {
  struct sh_identifier *f = sh_baseof(_f, struct sh_identifier, form);
  sh_puts(out, f->name);
}

static void identifier_free(struct sh_form *_f, struct sh_vm *vm) {
  struct sh_identifier *f = sh_baseof(_f, struct sh_identifier, form);
  sh_release(vm->malloc, f->name);
  sh_release(vm->malloc, f);
}

const struct sh_form_type SH_IDENTIFIER = {
  .dump = identifier_dump,
  .emit = identifier_emit,
  .free = identifier_free
};

void sh_identifier_init(struct sh_identifier *f,
			struct sh_vm *vm,
			const struct sh_sloc sloc,
			struct sh_list *owner,
			const char *name) {
  sh_form_init(&f->form, &SH_IDENTIFIER, sloc, owner);
  f->name = sh_strdup(name, vm->malloc);
}
