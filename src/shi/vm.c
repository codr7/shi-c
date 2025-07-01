#include <math.h>
#include <string.h>

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
		 sh_stack_t *stack,
		 const size_t start_pc,
		 const size_t end_pc) {
  const uint8_t *const ep = (end_pc == -1)
    ? vm->code.end
    : sh_vector_get(&vm->code, end_pc);

  for (uint8_t *p = sh_vector_get(&vm->code, start_pc);
       p != ep;
       p = (*(sh_evaluate_t *)p)(vm, stack, p + vm->code.item_size));
}

static void fun_dump(const struct sh_cell *v, struct sh_stream *out) {
  sh_printf(out, "%p", v->as_pointer);
}

const struct sh_type SH_VM_FUN = {
  .name = "VM/Fun",
  .copy = NULL,
  .dump = fun_dump
};

static uint8_t *call_eval(struct sh_vm *vm, sh_stack_t *stack, uint8_t *data) {
  struct sh_call_operation *op = (void *)sh_align(data, alignof(struct sh_call_operation));
  op->target(vm, op->sloc);
  return (uint8_t *)op + sizeof(struct sh_call_operation);
}

const struct sh_operation SH_CALL = (struct sh_operation){
  .name = "call",
  .align = alignof(struct sh_call_operation),
  .size = sizeof(struct sh_call_operation),
  .eval = call_eval,
  .deinit = NULL
};

static void push_deinit(uint8_t *data) {
  struct sh_push_operation *op = (void *)data;
  sh_cell_deinit(&op->value);
}

static uint8_t *push_eval(struct sh_vm *vm, sh_stack_t *stack, uint8_t *data) {
  struct sh_push_operation *op = (void *)sh_align(data, alignof(struct sh_push_operation));
  sh_cell_copy(sh_stack_push(stack), &op->value);
  return (uint8_t *)op + sizeof(struct sh_push_operation);
}

const struct sh_operation SH_PUSH = (struct sh_operation){
  .name = "push",
  .align = alignof(struct sh_push_operation),
  .size = sizeof(struct sh_push_operation),
  .eval = push_eval,
  .deinit = push_deinit
};

