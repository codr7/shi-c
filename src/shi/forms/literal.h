#ifndef SHI_FORMS_LITERAL
#define SHI_FORMS_LITERAL

#include "shi/form.h"
#include "shi/cell.h"

extern const struct sh_form_type SH_LITERAL;

struct sh_literal {
  struct sh_form form;
  struct sh_cell value;
};

void sh_literal_init(struct sh_literal *f,
		     struct sh_vm *vm,
		     struct sh_sloc sloc,
		     struct sh_list *owner);

#endif
