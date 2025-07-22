#include <stdint.h>

#include "shi/operations/goto.h"
#include "shi/stack.h"
#include "shi/vm.h"

static uint8_t *evaluate(struct sh_vm *vm,
			 struct sh_stack *stack,
			 uint8_t *data) {
  struct sh_goto *op = (void *)sh_align(data, alignof(struct sh_goto));
  return sh_pc_pointer(vm, op->target->pc);
}

const struct sh_operation SH_GOTO = (struct sh_operation){
  .name = "GOTO",
  .align = alignof(struct sh_goto),
  .size = sizeof(struct sh_goto),
  .evaluate = evaluate,
  .deinit = NULL
};
