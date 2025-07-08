#ifndef SHI_FORMS_IDENTIFIER
#define SHI_FORMS_IDENTIFIER

#include "shi/form.h"

extern const struct sh_form_type SH_IDENTIFIER;

struct sh_identifier {
  struct sh_form form;
  char *name;
};

void sh_identifier_init(struct sh_identifier *f,
			struct sh_vm *vm,
			struct sh_sloc sloc,
			struct sh_list *owner,
			const char *name);

#endif
