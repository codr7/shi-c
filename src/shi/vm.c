#include <math.h>
#include <string.h>

#include "shi/operation.h"
#include "shi/stack.h"
#include "shi/stream.h"
#include "shi/vm.h"

void sh_vm_init(struct sh_vm *vm, struct sh_malloc *malloc) {
  sh_vector_init(&vm->operations, malloc, sizeof(const struct sh_operation *));
  sh_vector_init(&vm->code, malloc, sizeof(sh_evaluate_t));
}

static size_t op_items(const struct sh_operation *op,
		       uint8_t *p,
		       struct sh_vm *vm) {
  const size_t s = op->size + sh_align(p, op->align) - p;
  return ceil(s / (double)vm->code.item_size);
}

static void deinit_operations(struct sh_vm *vm) {
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
}

void sh_vm_deinit(struct sh_vm *vm) {  
  deinit_operations(vm);
  sh_vector_deinit(&vm->code);
}

size_t sh_emit(struct sh_vm *vm,
	       const struct sh_operation *op,
	       const void *data) {
  *(const struct sh_operation **)sh_vector_push(&vm->operations) = op;
  const size_t pc = vm->code.length;
  *(sh_evaluate_t *)sh_vector_push(&vm->code) = op->eval;
  
  uint8_t *const p = sh_vector_insert(&vm->code,
				      vm->code.length,
				      op_items(op, vm->code.end, vm));
  
  memcpy(sh_align(p, op->align), data, op->size);
  return pc;
}

void sh_evaluate(struct sh_vm *vm,
		 struct sh_stack *stack,
		 const size_t start_pc,
		 const size_t end_pc) {
  const uint8_t *const ep = (end_pc == -1)
    ? vm->code.end
    : sh_vector_get(&vm->code, end_pc);

  for (uint8_t *p = sh_vector_get(&vm->code, start_pc);
       p != ep;
       p = (*(sh_evaluate_t *)p)(vm, stack, p + vm->code.item_size));
}
