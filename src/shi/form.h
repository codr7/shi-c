#ifndef SHI_FORM
#define SHI_FORM

#include "shi/list.h"
#include "shi/sloc.h"

struct sh_form;
struct sh_stream;
struct sh_vm;

struct sh_form_type {
  void (*dump)(const struct sh_form *, struct sh_stream *);
  void (*emit)(struct sh_form *, struct sh_vm *, struct sh_list *);
  void (*free)(struct sh_form *, struct sh_vm *);
};
  
struct sh_form {
  const struct sh_form_type *type;
  struct sh_sloc sloc;
  struct sh_list owner;
  int reference_count;
};

void sh_form_init(struct sh_form *f,
		  const struct sh_form_type *type,
		  struct sh_sloc sloc,
		  struct sh_list *owner);

struct sh_form *sh_form_acquire(struct sh_form *f);

void sh_form_dump(struct sh_form *f, struct sh_stream *out);
void sh_form_emit(struct sh_form *f, struct sh_vm *vm, struct sh_list *args);
void sh_form_release(struct sh_form *f, struct sh_vm *vm);

void sh_forms_dump(struct sh_list *in, struct sh_stream *out);
void sh_forms_emit(struct sh_list *in, struct sh_vm *vm);
void sh_forms_release(struct sh_list *in, struct sh_vm *vm);

#endif
