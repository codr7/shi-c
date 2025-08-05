#include <stdint.h>

#include "shi/call.h"
#include "shi/operation.h"
#include "shi/stack.h"
#include "shi/vm.h"

static uint8_t *evaluate(struct sh_vm *vm,
			 struct sh_stack *stack,
			 uint8_t *data) {
  return sh_pc_pointer(vm, sh_return(vm));
}


void sh_emit_return(struct sh_vm *vm) {
  static struct sh_operation op;
  static bool init = true;

  if (init) {
    sh_operation_init(&op, "RETURN", 0, 0);
    op.evaluate = evaluate;
    init = false;
  }

  sh_emit(vm, &op, NULL);
}
