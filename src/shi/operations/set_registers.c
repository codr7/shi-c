#include <stdint.h>

#include "shi/operations/set_registers.h"
#include "shi/stack.h"
#include "shi/vm.h"

static uint8_t *evaluate(struct sh_vm *vm,
			 struct sh_stack *stack,
			 uint8_t *data) {
  struct sh_set_registers *op =
    (void *)sh_align(data, alignof(struct sh_set_registers));

  for (int i = 0; i < op->count; i++) {
    struct sh_cell *v = sh_register(vm, op->r_target);
    sh_cell_deinit(v);
    *v = *sh_pop(stack);
  }
  
  return (uint8_t *)op + sizeof(struct sh_set_registers);
}

const struct sh_operation SH_SET_REGISTERS = (struct sh_operation){
  .name = "SET_REGISTERS",
  .align = alignof(struct sh_set_registers),
  .size = sizeof(struct sh_set_registers),
  .evaluate = evaluate,
  .deinit = NULL
};
