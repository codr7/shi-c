#include <stdint.h>

#include "shi/call.h"
#include "shi/operations/return.h"
#include "shi/stack.h"
#include "shi/vm.h"

static uint8_t *evaluate(struct sh_vm *vm,
			 struct sh_stack *stack,
			 uint8_t *data) {
  return sh_pc_pointer(vm, sh_return(vm));
}

const struct sh_operation SH_RETURN = (struct sh_operation){
  .name = "RETURN",
  .align = 0,
  .size = 0,
  .evaluate = evaluate,
  .deinit = NULL
};
