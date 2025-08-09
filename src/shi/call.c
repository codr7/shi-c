#include "shi/call.h"
#include "shi/library.h"
#include "shi/malloc.h"
#include "shi/method.h"
#include "shi/vm.h"

#include <string.h>

struct sh_call *sh_call_init(struct sh_call *c,
			     struct sh_shi_method *target,
			     struct sh_sloc *sloc,
			     size_t return_pc) {
  c->target = target;
  c->sloc = sloc;
  c->return_pc = return_pc;
  return c;
}

void sh_call(struct sh_shi_method *target,
	     struct sh_sloc *sloc,
	     size_t return_pc) {
  struct sh_vm *vm = target->method.vm;
  struct sh_call *c = sh_vector_push(&vm->call_stack);
  sh_call_init(c, target, sloc, return_pc);

  memmove(c->arguments,
	  vm->registers.start + target->r_arguments * sizeof(struct sh_cell),
	  sizeof(struct sh_cell) * target->method.arity);
}

size_t sh_return(struct sh_vm *vm) {
  struct sh_call *c = sh_vector_pop(&vm->call_stack);

  memmove(vm->registers.start + c->target->r_arguments * sizeof(struct sh_cell),
	  c->arguments,
	  sizeof(struct sh_cell) * c->target->method.arity);
  
  return c->return_pc;
}
