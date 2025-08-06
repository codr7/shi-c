#include <stdint.h>

#include "shi/chrono.h"
#include "shi/error.h"
#include "shi/libraries/core.h"
#include "shi/operation.h"
#include "shi/stack.h"
#include "shi/type.h"
#include "shi/vm.h"

struct sh_benchmark {
  int rounds;
  struct sh_label *end;
};

static uint8_t *evaluate(struct sh_vm *vm,
			 struct sh_stack *stack,
			 uint8_t *data) {
  struct sh_benchmark *op =
    (void *)sh_align(data, __alignof(struct sh_benchmark));

  struct sh_stack s;
  sh_stack_init(&s, vm->malloc);
  const sh_time_t t = sh_now();
  const int body_pc = sh_pointer_pc(vm, (uint8_t *)op + sizeof(struct sh_benchmark));
  
  for (int i = 0; i < op->rounds; i++) {
    sh_evaluate(vm, &s, body_pc, op->end->pc);
    sh_stack_clear(&s);
  }

  sh_cell_init(sh_push(stack), SH_INT())->as_int = sh_time_ns(&t);
  sh_stack_deinit(&s);
  return sh_pc_pointer(vm, op->end->pc);
}


void sh_emit_benchmark(struct sh_vm *vm,
		       int rounds,
		       struct sh_label *end) {
  static struct sh_operation op;
  static bool init = true;

  if (init) {
    sh_operation_init(&op,
		      "BENCHMARK",
		      sizeof(struct sh_benchmark),
		      __alignof(struct sh_benchmark));

    op.evaluate = evaluate;
    init = false;
  }
  
  sh_emit(vm, &op, &(struct sh_benchmark) {
      .rounds = rounds,
      .end = end
    });
}
					  
