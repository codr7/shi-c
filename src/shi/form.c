#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "shi/error.h"
#include "shi/form.h"
#include "shi/malloc.h"
#include "shi/stream.h"
#include "shi/vm.h"

void sh_form_init(struct sh_form *f,
		  const struct sh_form_type *t,
		  const struct sh_sloc sloc,
		  struct sh_list *owner) {
  f->type = t;
  f->sloc = sloc;

  if (owner) {
    sh_list_push_back(owner, &f->owner);
  } else {
    sh_list_init(&f->owner);
  }
}

void sh_form_dump(struct sh_form *f, struct sh_stream *out) {
  assert(f->type->dump);
  f->type->dump(f, out);
}

void sh_form_emit(struct sh_form *f, struct sh_vm *vm) {
  assert(f->type->emit);
  f->type->emit(f, vm);
}

void sh_form_free(struct sh_form *f, struct sh_vm *vm) {
  sh_list_delete(&f->owner);
  assert(f->type->free);
  f->type->free(f, vm);
}

static void id_emit(struct sh_form *_f, struct sh_vm *vm) {
  /*
  struct sh_fid *f = sh_baseof(_f, struct sh_fid, form);
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

static void id_dump(const struct sh_form *_f, struct sh_stream *out) {
  struct sh_fid *f = sh_baseof(_f, struct sh_fid, form);
  sh_puts(out, f->name);
}

static void id_free(struct sh_form *_f, struct sh_vm *vm) {
  struct sh_fid *f = sh_baseof(_f, struct sh_fid, form);
  free(f->name);
  _sh_release(vm->malloc, f);
}

const struct sh_form_type SH_FID = {
  .dump = id_dump,
  .emit = id_emit,
  .free = id_free
};

void sh_fid_init(struct sh_fid *f,
		 const struct sh_sloc sloc,
		 struct sh_list *owner,
		 const char *name) {
  sh_form_init(&f->form, &SH_FID, sloc, owner);
  f->name = strdup(name);
}
