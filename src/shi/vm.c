#include <math.h>
#include <string.h>

#include "shi/libraries/core.h"
#include "shi/operation.h"
#include "shi/stack.h"
#include "shi/stream.h"
#include "shi/vm.h"

void sh_vm_init(struct sh_vm *vm, struct sh_malloc *malloc) {
  vm->malloc = malloc;
  sh_vector_init(&vm->operations, malloc, sizeof(const struct sh_operation *));
  sh_vector_init(&vm->code, malloc, sizeof(sh_evaluate_t));
  sh_library_init(&vm->user_library, vm, "user", NULL);
  sh_core_library_init(&vm->core_library, vm);
  vm->library = &vm->user_library;
  sh_import(vm->library, &vm->core_library);
}

static size_t op_items(const struct sh_operation *op,
		       uint8_t *p,
		       struct sh_vm *vm) {
  const size_t s = op->size + sh_align(p, op->align) - p;
  return ceil(s / (double)vm->code.item_size);
}

static void deinit_code(struct sh_vm *vm) {
  uint8_t *p = vm->code.start;
  
  sh_vector_do(&vm->operations, _operation) {
    const struct sh_operation *op = *(const struct sh_operation **)_operation;
    p += sizeof(sh_evaluate_t);

    if (op->deinit) {
      op->deinit(sh_align(p, op->align));
    }

    p += op_items(op, p, vm) * vm->code.item_size;
  }

  sh_vector_deinit(&vm->operations);
  sh_vector_deinit(&vm->code);
}

void sh_vm_deinit(struct sh_vm *vm) {  
  deinit_code(vm);
}

size_t sh_emit(struct sh_vm *vm,
	       const struct sh_operation *op,
	       const void *data) {
  *(const struct sh_operation **)sh_vector_push(&vm->operations) = op;
  const size_t pc = vm->code.length;
  *(sh_evaluate_t *)sh_vector_push(&vm->code) = op->evaluate;
  
  uint8_t *const p = sh_vector_insert(&vm->code,
				      vm->code.length,
				      op_items(op, vm->code.end, vm));
  
  memcpy(sh_align(p, op->align), data, op->size);
  return pc;
}

size_t sh_emit_pc(struct sh_vm *vm) {
  return vm->code.length;
}

void sh_evaluate(struct sh_vm *vm,
		 struct sh_stack *stack,
		 const size_t start,
		 const size_t end) {
  const uint8_t *end_pc = (end == -1)
    ? vm->code.end
    : sh_vector_get(&vm->code, end);
  
  for (uint8_t *pc = sh_vector_get(&vm->code, start);
       pc != end_pc;
       pc = (*(sh_evaluate_t *)pc)(vm, stack, pc + vm->code.item_size)) {
  }
}
