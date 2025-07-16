#include "shi/error.h"
#include "shi/evaluate.h"
#include "shi/libraries/core.h"
#include "shi/method.h"
#include "shi/stack.h"
#include "shi/type.h"
#include "shi/vm.h"

static uint8_t *call_method_evaluate(struct sh_vm *vm,
				     struct sh_stack *stack,
				     uint8_t *data) {
  struct sh_call_method *op =
    (void *)sh_align(data, alignof(struct sh_call_method));

  struct sh_method *m = op->target;

  if (stack->items.length < m->arity) {
    sh_throw("Error in %s: Not enough arguments", sh_sloc_string(&op->sloc));
  }

  for (int i = 0; i < m->arity; i++) {
    struct sh_cell *v = sh_vector_get(&stack->items, stack->items.length - i - 1);
    const struct sh_type *at = m->arguments[i].type;

    if (at != SH_ANY() && !sh_isa(v, at)) {
      sh_throw("Error in %s: Type mismatch; expected %s, actual %s",
	       sh_sloc_string(&op->sloc), at->name, v->type->name);
    }
  }

  size_t pc = sh_pointer_pc(vm, (uint8_t *)op + sizeof(struct sh_call_method));
  sh_method_call(m, &pc, stack,  &op->sloc);  
  return sh_pc_pointer(vm, pc);
}

const struct sh_operation SH_CALL_METHOD = (struct sh_operation){
  .name = "CALL_METHOD",
  .align = alignof(struct sh_call_method),
  .size = sizeof(struct sh_call_method),
  .evaluate = call_method_evaluate,
  .deinit = NULL
};

static void push_value_deinit(uint8_t *data) {
  struct sh_push_value *op = (void *)data;
  sh_cell_deinit(&op->value);
}

static uint8_t *push_value_evaluate(struct sh_vm *vm,
				    struct sh_stack *stack,
				    uint8_t *data) {
  struct sh_push_value *op = (void *)sh_align(data, alignof(struct sh_push_value));
  sh_cell_copy(sh_push(stack), &op->value, vm);
  return (uint8_t *)op + sizeof(struct sh_push_value);
}

const struct sh_operation SH_PUSH_VALUE = (struct sh_operation){
  .name = "PUSH_VALUE",
  .align = alignof(struct sh_push_value),
  .size = sizeof(struct sh_push_value),
  .evaluate = push_value_evaluate,
  .deinit = push_value_deinit
};
