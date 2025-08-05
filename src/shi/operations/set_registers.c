#include <stdint.h>

#include "shi/operations/set_registers.h"
#include "shi/stack.h"
#include "shi/vm.h"

struct sh_set_registers {
  size_t r_target;
  int count;
};

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

void sh_emit_set_registers(struct sh_vm *vm,
			   const size_t r_target,
			   const int count) {
  static struct sh_operation op;
  static bool init = true;

  if (init) {
    sh_operation_init(&op,
		      "SET_REGISTERS",
		      sizeof(struct sh_set_registers),
		      alignof(struct sh_set_registers));
    
    op.evaluate = evaluate;
    init = false;
  }

  sh_emit(vm, &op, &(struct sh_set_registers) {
      .r_target = r_target,
      .count = count
    });
}
