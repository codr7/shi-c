#include "shi/error.h"
#include "shi/operation.h"
#include "shi/stack.h"
#include "shi/stream.h"
#include "shi/vm.h"

struct sh_check_value {
  struct sh_cell expected;
  struct sh_sloc sloc;
};

static void deinit(uint8_t *data) {
  struct sh_check_value *op = (void *)data;
  sh_cell_deinit(&op->expected);
}

static uint8_t *evaluate(struct sh_vm *vm,
			 struct sh_stack *stack,
			 uint8_t *data) {
  struct sh_check_value *op = (void *)sh_align(data, __alignof(struct sh_check_value));
  struct sh_cell *actual = sh_pop(stack);

  if (!sh_eq(&op->expected, actual)) {
    struct sh_memory_stream s;
    sh_memory_stream_init(&s, vm->malloc);
    sh_defer(sh_stream_deinit(&s.stream));
    sh_puts(&s.stream, "expected ");
    sh_cell_dump(&op->expected, &s.stream);
    sh_puts(&s.stream, ", actual ");
    sh_cell_dump(actual, &s.stream);
    
    sh_throw("Check failed in %s: %s",
	     sh_sloc_string(&op->sloc),
	     sh_memory_stream_string(&s));
  }
  
  return (uint8_t *)op + sizeof(struct sh_check_value);
}


void sh_emit_check_value(struct sh_vm *vm,
			 struct sh_cell *expected,
			 struct sh_sloc sloc) {
  static struct sh_operation op;
  static bool init = true;

  if (init) {
    sh_operation_init(&op,
		      "CHECK_VALUE",
		      sizeof(struct sh_check_value),
		      __alignof(struct sh_check_value));
    
    op.evaluate = evaluate;
    op.deinit = deinit;
    init = false;
  }

  struct sh_check_value d = {
    .sloc = sloc
  };

  sh_cell_copy(&d.expected, expected, vm);
  sh_emit(vm, &op, &d);
}
