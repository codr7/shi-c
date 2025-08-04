#include <stdint.h>

#include "shi/operations/branch.h"
#include "shi/stack.h"
#include "shi/stream.h"
#include "shi/vm.h"

struct sh_branch {
  struct sh_label *end;
};

static uint8_t *evaluate(struct sh_vm *vm,
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


void sh_emit_branch(struct sh_vm *vm, struct sh_label *end) {
  static struct sh_operation op;
  static bool init = true;

  if (init) {
    sh_operation_init(&op,
		      "BRANCH",
		      sizeof(struct sh_branch),
		      alignof(struct sh_branch));
    
    op.evaluate = evaluate;
    init = false;
  }
  
  sh_emit(vm, &op, &(struct sh_branch) { .end = end });
}
