#ifndef SHI_FORM
#define SHI_FORM

#include "shi/list.h"
#include "shi/sloc.h"

struct sh_form;
struct sh_stream;
struct sh_vm;

struct sh_form_type {
  void (*dump)(const struct sh_form *, struct sh_stream *);
  void (*emit)(struct sh_form *, struct sh_vm *);
  void (*free)(struct sh_form *, struct sh_vm *);
};
  
struct sh_form {
  const struct sh_form_type *type;
  struct sh_sloc sloc;
  struct sh_list owner;
};

void sh_form_init(struct sh_form *f,
		  const struct sh_form_type *type,
		  struct sh_sloc sloc,
		  struct sh_list *owner);

void sh_form_dump(struct sh_form *f, struct sh_stream *out);
void sh_form_emit(struct sh_form *f, struct sh_vm *vm);
void sh_form_free(struct sh_form *f, struct sh_vm *vm);

extern const struct sh_form_type SH_FID;

struct sh_fid {
  struct sh_form form;
  char *name;
};

void sh_fid_init(struct sh_fid *f,
		 struct sh_sloc sloc,
		 struct sh_list *owner,
		 const char *name);

#endif
