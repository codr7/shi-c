#include <stdint.h>

#include "shi/operations/goto.h"
#include "shi/stack.h"
#include "shi/vm.h"

struct sh_goto {
  struct sh_label *target;
};

static uint8_t *evaluate(struct sh_vm *vm,
			 struct sh_stack *stack,
			 uint8_t *data) {
  struct sh_goto *op = (void *)sh_align(data, alignof(struct sh_goto));
  return sh_pc_pointer(vm, op->target->pc);
}

void sh_emit_goto(struct sh_vm *vm, struct sh_label *target) {
  static struct sh_operation op;
  static bool init = true;

  if (init) {
    sh_operation_init(&op,
		      "GOTO",
		      sizeof(struct sh_goto),
		      alignof(struct sh_goto));
    op.evaluate = evaluate;
  }

  sh_emit(vm, &op, &(struct sh_goto) { .target = target });
}
