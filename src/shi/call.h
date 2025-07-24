#ifndef SHI_CALL
#define SHI_CALL

#include <stddef.h>

struct sh_vm;

struct sh_call {
  struct sh_call *parent;
  size_t return_pc;
  struct sh_sloc *sloc;
  struct sh_shi_method *target;
  struct sh_cell *arguments;
};

struct sh_call *sh_call_init(struct sh_call *c,
			     struct sh_call *parent,
			     struct sh_shi_method *target,
			     struct sh_sloc *sloc,
			     size_t return_pc);

void sh_call(struct sh_shi_method *target,
	     struct sh_sloc *sloc,
	     size_t return_pc);

size_t sh_return(struct sh_vm *vm);

#endif
