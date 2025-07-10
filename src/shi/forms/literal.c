#include "shi/error.h"
#include "shi/evaluate.h"
#include "shi/forms/literal.h"
#include "shi/library.h"
#include "shi/malloc.h"
#include "shi/stream.h"
#include "shi/vm.h"

static void literal_emit(struct sh_form *_f,
			 struct sh_vm *vm,
			 struct sh_list *args) {
  struct sh_literal *f = sh_baseof(_f, struct sh_literal, form);
  struct sh_push_value op;
  sh_cell_copy(&op.value, &f->value, vm);
  sh_emit(vm, &SH_PUSH_VALUE, &op);
}

static void literal_dump(const struct sh_form *_f, struct sh_stream *out) {
  struct sh_literal *f = sh_baseof(_f, struct sh_literal, form);
  sh_cell_dump(&f->value, out);
}

static void literal_free(struct sh_form *_f, struct sh_vm *vm) {
  struct sh_literal *f = sh_baseof(_f, struct sh_literal, form);  
  sh_cell_deinit(&f->value);
  sh_release(vm->malloc, f);
}

const struct sh_form_type SH_LITERAL = {
  .dump = literal_dump,
  .emit = literal_emit,
  .free = literal_free
};

void sh_literal_init(struct sh_literal *f,
		     struct sh_vm *vm,
		     const struct sh_sloc sloc,
		     struct sh_list *owner) {
  sh_form_init(&f->form, &SH_LITERAL, sloc, owner);
}
