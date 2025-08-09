#ifndef SHI_CALL
#define SHI_CALL

#include <stddef.h>

#include "shi/cell.h"

struct sh_vm;

#define SH_MAX_ARITY 8

struct sh_call {
  size_t return_pc;
  struct sh_sloc *sloc;
  struct sh_shi_method *target;
  struct sh_cell arguments[SH_MAX_ARITY];
};

struct sh_call *sh_call_init(struct sh_call *c,
			     struct sh_shi_method *target,
			     struct sh_sloc *sloc,
			     size_t return_pc);

void sh_call(struct sh_shi_method *target,
	     struct sh_sloc *sloc,
	     size_t return_pc);

size_t sh_return(struct sh_vm *vm);

#endif
