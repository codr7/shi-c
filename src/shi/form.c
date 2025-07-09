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

void sh_form_emit(struct sh_form *f, struct sh_vm *vm, struct sh_list *args) {
  assert(f->type->emit);
  f->type->emit(f, vm, args);
}

void sh_form_free(struct sh_form *f, struct sh_vm *vm) {
  sh_list_delete(&f->owner);
  assert(f->type->free);
  f->type->free(f, vm);
}

void sh_forms_dump(struct sh_list *in, struct sh_stream *out) {
  sh_list_do(in, f) {
    if (f != in->next) {
      sh_putc(out, ' ');
    }
    
    sh_form_dump(sh_baseof(f, struct sh_form, owner), out);
  }
}

void sh_forms_emit(struct sh_list *in, struct sh_vm *vm) {
  while (in->next != in) {
    struct sh_form *f = sh_baseof(sh_list_pop_front(in), struct sh_form, owner);
    sh_form_emit(f, vm, in);
  }
}

void sh_forms_free(struct sh_list *in, struct sh_vm *vm) {
  sh_list_do(in, f) {
    sh_form_free(sh_baseof(f, struct sh_form, owner), vm);
  }
}
