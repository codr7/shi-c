#include <stdint.h>

#include "shi/error.h"
#include "shi/libraries/core.h"
#include "shi/operation.h"
#include "shi/stack.h"
#include "shi/type.h"
#include "shi/vm.h"

struct sh_call_method {
  struct sh_method *target;
  struct sh_sloc sloc;
};

static void deinit(uint8_t *data) {
  struct sh_call_method *op =
    (void *)sh_align(data, __alignof(struct sh_call_method));

  sh_method_release(op->target);
}

static uint8_t *evaluate(struct sh_vm *vm,
			 struct sh_stack *stack,
			 uint8_t *data) {
  struct sh_call_method *op =
    (void *)sh_align(data, __alignof(struct sh_call_method));

  struct sh_method *m = op->target;

  if (stack->items.length < m->arity) {
    sh_throw("Error in %s: Not enough arguments", sh_sloc_string(&op->sloc));
  }

  struct sh_cell *v = sh_vector_get(&stack->items, stack->items.length - 1);

  for (int i = 0; i < m->arity; i++, v--) {
    const struct sh_type *at = m->arguments[i].type;

    if (at != SH_ANY() && !sh_isa(v, at)) {
      sh_throw("Error in %s: Type mismatch; expected %s, actual %s",
	       sh_sloc_string(&op->sloc), at->name, v->type->name);
    }
  }

  uint8_t *return_op = (uint8_t *)op + sizeof(struct sh_call_method);
  sh_method_call(m, &return_op, stack,  &op->sloc);  
  return return_op;
}

void sh_emit_call_method(struct sh_vm *vm,
			 struct sh_method *target,
			 struct sh_sloc *sloc) {
  static struct sh_operation op;
  static bool init = true;

  if (init) {
    sh_operation_init(&op,
		      "CALL_METHOD",
		      sizeof(struct sh_call_method),
		      __alignof(struct sh_call_method));
    
    op.deinit = deinit;
    op.evaluate = evaluate;
    init = false;
  }
  
  sh_emit(vm, &op, &(struct sh_call_method) {
    .target = sh_method_acquire(target),
    .sloc = *sloc
    });
}
					  
