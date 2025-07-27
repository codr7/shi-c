#include <stdint.h>

#include "shi/operations/get_register.h"
#include "shi/stack.h"
#include "shi/vm.h"

static uint8_t *evaluate(struct sh_vm *vm,
			 struct sh_stack *stack,
			 uint8_t *data) {
  struct sh_get_register *op =
    (void *)sh_align(data, alignof(struct sh_get_register));
  
  for (int i = 0; i < op->count; i++) {
    sh_cell_copy(sh_push(stack), sh_register(vm, op->r_source + i), vm);
  }
  
  return (uint8_t *)op + sizeof(struct sh_get_register);
}

const struct sh_operation SH_GET_REGISTER = (struct sh_operation){
  .name = "GET_REGISTER",
  .align = alignof(struct sh_get_register),
  .size = sizeof(struct sh_get_register),
  .evaluate = evaluate,
  .deinit = NULL
};
