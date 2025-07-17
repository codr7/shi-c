#include "shi/operations/push_value.h"
#include "shi/stack.h"
#include "shi/vm.h"

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
