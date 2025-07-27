#include <stdint.h>

#include "shi/operations/set_register.h"
#include "shi/stack.h"
#include "shi/vm.h"

static uint8_t *evaluate(struct sh_vm *vm,
			 struct sh_stack *stack,
			 uint8_t *data) {
  struct sh_set_register *op =
    (void *)sh_align(data, alignof(struct sh_set_register));

  for (int i = 0; i < op->count; i++) {
    struct sh_cell *v = sh_register(vm, op->r_target);
    sh_cell_deinit(v);
    *v = *sh_pop(stack);
  }
  
  return (uint8_t *)op + sizeof(struct sh_set_register);
}

const struct sh_operation SH_SET_REGISTER = (struct sh_operation){
  .name = "SET_REGISTER",
  .align = alignof(struct sh_set_register),
  .size = sizeof(struct sh_set_register),
  .evaluate = evaluate,
  .deinit = NULL
};
