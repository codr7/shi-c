#ifndef SHI_READ
#define SHI_READ

struct sh_list;
struct sh_sloc;
struct sh_vm;

bool sh_read_form(struct sh_vm *vm,
		  const char **in,
		  struct sh_list *out,
		  struct sh_sloc *sloc);

bool sh_read_forms(struct sh_vm *vm,
		   const char **in,
		   struct sh_list *out,
		   struct sh_sloc *sloc);

bool sh_read_identifier(struct sh_vm *vm,
			const char **in,
			struct sh_list *out,
			struct sh_sloc *sloc);

bool sh_read_int(struct sh_vm *vm,
		 const char **in,
		 struct sh_list *out,
		 struct sh_sloc *sloc);

void sh_read_whitespace(const char **in, struct sh_sloc *sloc);

#endif
