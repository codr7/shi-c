#include <stdint.h>

#include "shi/operations/get_registers.h"
#include "shi/stack.h"
#include "shi/vm.h"

struct sh_get_registers {
  size_t r_source;
  int count;
};

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

void sh_emit_get_registers(struct sh_vm *vm,
			   const size_t r_source,
			   const int count) {
  static struct sh_operation op;
  static bool init = true;

  if (init) {
    sh_operation_init(&op,
		      "GET_REGISTERS",
		      sizeof(struct sh_get_registers),
		      alignof(struct sh_get_registers));
    
    op.evaluate = evaluate;
    init = false;
  }

  sh_emit(vm, &op, &(struct sh_get_registers) {
      .r_source = r_source,
      .count = count
    });
}
