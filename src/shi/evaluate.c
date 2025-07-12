#include "shi/evaluate.h"
#include "shi/method.h"
#include "shi/stack.h"
#include "shi/vm.h"

static uint8_t *call_method_evaluate(struct sh_vm *vm,
				     struct sh_stack *stack,
				     uint8_t *data) {
  struct sh_call_method *op = (void *)sh_align(data, alignof(struct sh_call_method));
  size_t pc = ((uint8_t *)op + sizeof(struct sh_call_method) - vm->code.start) /
    vm->code.item_size;
  sh_method_call(op->target, &pc, stack,  &op->sloc);
  return sh_vector_get(&vm->code, pc);
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
