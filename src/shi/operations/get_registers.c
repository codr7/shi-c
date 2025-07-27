#include <stdint.h>

#include "shi/operations/get_registers.h"
#include "shi/stack.h"
#include "shi/vm.h"

static uint8_t *evaluate(struct sh_vm *vm,
			 struct sh_stack *stack,
			 uint8_t *data) {
  struct sh_get_registers *op =
    (void *)sh_align(data, alignof(struct sh_get_registers));
  
  for (int i = 0; i < op->count; i++) {
    sh_cell_copy(sh_push(stack), sh_register(vm, op->r_source + i), vm);
  }
  
  return (uint8_t *)op + sizeof(struct sh_get_registers);
}

const struct sh_operation SH_GET_REGISTERS = (struct sh_operation){
  .name = "GET_REGISTERS",
  .align = alignof(struct sh_get_registers),
  .size = sizeof(struct sh_get_registers),
  .evaluate = evaluate,
  .deinit = NULL
};
