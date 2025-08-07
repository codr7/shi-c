#include "shi/call.h"
#include "shi/library.h"
#include "shi/malloc.h"
#include "shi/method.h"
#include "shi/vm.h"

#include <string.h>

struct sh_call *sh_call_init(struct sh_call *c,
			     struct sh_call *parent,
			     struct sh_shi_method *target,
			     struct sh_sloc *sloc,
			     size_t return_pc) {
  c->parent = parent;
  c->target = target;
  c->sloc = sloc;
  c->return_pc = return_pc;
  return c;
}

void sh_call(struct sh_shi_method *target,
	     struct sh_sloc *sloc,
	     size_t return_pc) {
  struct sh_call *c = NULL;
  struct sh_vm *vm = target->method.vm;
  
  if (vm->call_cache) {
    c = vm->call_cache;
    vm->call_cache = c->parent;
  } else {
    c = sh_acquire(vm->malloc, sizeof(struct sh_call));
  }

  sh_call_init(c, vm->call_stack, target, sloc, return_pc);

  memmove(c->arguments,
	  vm->registers.start + target->r_arguments * sizeof(struct sh_cell),
	  sizeof(struct sh_cell) * target->method.arity);
  
  vm->call_stack = c;
}

size_t sh_return(struct sh_vm *vm) {
  struct sh_call *c = vm->call_stack;

  memmove(vm->registers.start + c->target->r_arguments * sizeof(struct sh_cell),
	  c->arguments,
	  sizeof(struct sh_cell) * c->target->method.arity);
  
  vm->call_stack = c->parent;
  c->parent = vm->call_cache;
  vm->call_cache = c;
  return c->return_pc;
}
