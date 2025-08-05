#include "shi/operations/push_value.h"
#include "shi/stack.h"
#include "shi/vm.h"

struct sh_push_value {
  struct sh_cell value;
};

static void deinit(uint8_t *data) {
  struct sh_push_value *op = (void *)data;
  sh_cell_deinit(&op->value);
}

static uint8_t *evaluate(struct sh_vm *vm,
			 struct sh_stack *stack,
			 uint8_t *data) {
  struct sh_push_value *op = (void *)sh_align(data, alignof(struct sh_push_value));
  sh_cell_copy(sh_push(stack), &op->value, vm);
  return (uint8_t *)op + sizeof(struct sh_push_value);
}

void sh_emit_push_value(struct sh_vm *vm, struct sh_cell *value) {
  static struct sh_operation op;
  static bool init = true;
  
  if (init) {
    sh_operation_init(&op,
		      "PUSH_VALUE",
		      sizeof(struct sh_push_value),
		      alignof(struct sh_push_value));
    op.evaluate = evaluate;
    op.deinit = deinit;
    init = false;
  }

  struct sh_push_value d;
  sh_cell_copy(&d.value, value, vm);
  sh_emit(vm, &op, &d);
}

