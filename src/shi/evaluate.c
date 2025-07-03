#include "shi/evaluate.h"
#include "shi/method.h"
#include "shi/stack.h"

static uint8_t *call_eval(struct sh_vm *vm, struct sh_stack *stack, uint8_t *data) {
  struct sh_call_operation *op = (void *)sh_align(data, alignof(struct sh_call_operation));
  op->target->call(op->target, vm,stack,  op->sloc);
  return (uint8_t *)op + sizeof(struct sh_call_operation);
}

const struct sh_operation SH_CALL = (struct sh_operation){
  .name = "call",
  .align = alignof(struct sh_call_operation),
  .size = sizeof(struct sh_call_operation),
  .eval = call_eval,
  .deinit = NULL
};

static void push_deinit(uint8_t *data) {
  struct sh_push_operation *op = (void *)data;
  sh_cell_deinit(&op->value);
}

static uint8_t *push_eval(struct sh_vm *vm, struct sh_stack *stack, uint8_t *data) {
  struct sh_push_operation *op = (void *)sh_align(data, alignof(struct sh_push_operation));
  sh_cell_copy(sh_push(stack), &op->value);
  return (uint8_t *)op + sizeof(struct sh_push_operation);
}

const struct sh_operation SH_PUSH = (struct sh_operation){
  .name = "push",
  .align = alignof(struct sh_push_operation),
  .size = sizeof(struct sh_push_operation),
  .eval = push_eval,
  .deinit = push_deinit
};
