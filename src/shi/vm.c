#include <math.h>
#include <string.h>

#include "shi/call.h"
#include "shi/form.h"
#include "shi/libraries/core.h"
#include "shi/malloc.h"
#include "shi/operation.h"
#include "shi/read.h"
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
  sh_list_init(&vm->labels);
  sh_vector_init(&vm->registers, malloc, sizeof(struct sh_cell));
  sh_vector_init(&vm->call_stack, malloc, sizeof(struct sh_call));
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

static void deinit_registers(struct sh_vm *vm) {
  sh_vector_do(&vm->registers, v) {
    sh_cell_deinit(v);
  }
  
  sh_vector_deinit(&vm->registers);
}

void sh_vm_deinit(struct sh_vm *vm) {  
  deinit_code(vm);
  sh_library_deinit(&vm->core_library);
  sh_library_deinit(&vm->user_library);
  
  sh_list_do(&vm->labels, l) {
    sh_release(vm->malloc, sh_baseof(l, struct sh_label, owner));
  }

  sh_vector_deinit(&vm->call_stack);
  deinit_registers(vm);
}

size_t sh_allocate_registers(struct sh_vm *vm, const size_t n) {
  const size_t result = vm->registers.length;
  
  for (size_t i = 0; i < n; i++) {
    sh_cell_init(sh_vector_push(&vm->registers), SH_INT())->as_int = 0;
  }

  return result;
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

  if (op->size) {
    memcpy(sh_align(p, op->align), data, op->size);
  }
  
  return pc;
}

size_t sh_emit_pc(struct sh_vm *vm) {
  return vm->code.length;
}

void sh_evaluate(struct sh_vm *vm,
		 struct sh_stack *stack,
		 const size_t start_pc,
		 const size_t end_pc) {
  const uint8_t *end = (end_pc == -1)
    ? vm->code.end
    : sh_vector_get(&vm->code, end_pc);
  
  for (uint8_t *p = sh_vector_get(&vm->code, start_pc);
       p != end;
       p = (*(sh_evaluate_t *)p)(vm, stack, p + vm->code.item_size)) {
  }
}

struct sh_label *sh_label(struct sh_vm *vm, const size_t pc) {
  struct sh_label *l = sh_acquire(vm->malloc, sizeof(struct sh_label));
  l->pc = pc;
  sh_list_push_back(&vm->labels, &l->owner);
  return l;
}

void sh_load(struct sh_vm *vm, const char *path) {
  struct sh_sloc sloc = sh_sloc(path, 0, 0);
  char *code = sh_slurp(path, vm->malloc);
  sh_defer(sh_release(vm->malloc, code));

  struct sh_list forms;
  sh_list_init(&forms);

  const char *c = code;
  sh_read_forms(vm, &c, &forms, &sloc);
  sh_defer(sh_forms_release(&forms, vm));
  sh_forms_emit(&forms, vm);
}

size_t sh_pointer_pc(struct sh_vm *vm, const uint8_t *p) {
  return (p - vm->code.start) / vm->code.item_size;
}

uint8_t *sh_pc_pointer(struct sh_vm *vm, const size_t pc) {
  return sh_vector_get(&vm->code, pc); 
}

struct sh_cell *sh_register(struct sh_vm *vm, const size_t r_target) {
  return sh_vector_get(&vm->registers, r_target);
}
