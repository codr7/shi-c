#include "shi/evaluate.h"
#include "shi/method.h"
#include "shi/stack.h"

static uint8_t *call_method_evaluate(struct sh_vm *vm,
				     struct sh_stack *stack,
				     uint8_t *data) {
  struct sh_call_method *op = (void *)sh_align(data, alignof(struct sh_call_method));
  op->target->call(op->target, vm,stack,  &op->sloc);
  return (uint8_t *)op + sizeof(struct sh_call_method);
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
