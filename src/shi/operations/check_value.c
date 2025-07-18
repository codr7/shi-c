#include "shi/error.h"
#include "shi/operations/check_value.h"
#include "shi/stack.h"
#include "shi/vm.h"

static void deinit(uint8_t *data) {
  struct sh_check_value *op = (void *)data;
  sh_cell_deinit(&op->expected);
}

static uint8_t *evaluate(struct sh_vm *vm,
			 struct sh_stack *stack,
			 uint8_t *data) {
  struct sh_check_value *op = (void *)sh_align(data, alignof(struct sh_check_value));
  struct sh_cell *actual = sh_pop(stack);
  
  if (!sh_eq(&op->expected, actual)) {
    sh_throw("Check_Value failed in %s", sh_sloc_string(&op->sloc));
  }
  
  return (uint8_t *)op + sizeof(struct sh_check_value);
}

const struct sh_operation SH_CHECK_VALUE = (struct sh_operation){
  .name = "CHECK_VALUE",
  .align = alignof(struct sh_check_value),
  .size = sizeof(struct sh_check_value),
  .evaluate = evaluate,
  .deinit = deinit
};
