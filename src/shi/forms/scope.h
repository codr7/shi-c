#ifndef SHI_FORMS_SCOPE
#define SHI_FORMS_SCOPE

#include "shi/form.h"
#include "shi/cell.h"

extern const struct sh_form_type SH_SCOPE;

struct sh_scope {
  struct sh_form form;
  struct sh_list items;
};

void sh_scope_init(struct sh_scope *f,
		  struct sh_vm *vm,
		  struct sh_sloc sloc,
		  struct sh_list *owner);

#endif
