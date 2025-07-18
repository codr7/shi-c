#include "shi/error.h"
#include "shi/operations/check.h"
#include "shi/stack.h"
#include "shi/vm.h"

static void check_deinit(uint8_t *data) {
  struct sh_check *op = (void *)data;
  sh_cell_deinit(&op->expected);
}

static uint8_t *check_evaluate(struct sh_vm *vm,
				    struct sh_stack *stack,
				    uint8_t *data) {
  struct sh_check *op = (void *)sh_align(data, alignof(struct sh_check));
  struct sh_cell *actual = sh_pop(stack);
  
  if (!sh_eq(&op->expected, actual)) {
    sh_throw("Check failed in %s", sh_sloc_string(&op->sloc));
  }
  
  return (uint8_t *)op + sizeof(struct sh_check);
}

const struct sh_operation SH_CHECK = (struct sh_operation){
  .name = "CHECK",
  .align = alignof(struct sh_check),
  .size = sizeof(struct sh_check),
  .evaluate = check_evaluate,
  .deinit = check_deinit
};
