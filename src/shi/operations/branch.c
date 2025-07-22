#include <stdint.h>

#include "shi/operations/branch.h"
#include "shi/stack.h"
#include "shi/vm.h"

static uint8_t *branch_evaluate(struct sh_vm *vm,
				struct sh_stack *stack,
				uint8_t *data) {
  struct sh_branch *op =
    (void *)sh_align(data, alignof(struct sh_branch));

  struct sh_cell *v = sh_pop(stack);
  bool c = sh_as_bool(v);
  sh_cell_deinit(v);
  
  return c
    ? (uint8_t *)op + sizeof(struct sh_branch)
    : sh_pc_pointer(vm, op->end->pc);
}

const struct sh_operation SH_BRANCH = (struct sh_operation){
  .name = "BRANCH",
  .align = alignof(struct sh_branch),
  .size = sizeof(struct sh_branch),
  .evaluate = branch_evaluate,
  .deinit = NULL
};
