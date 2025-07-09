#ifndef SHI_VM
#define SHI_VM

#include "shi/cell.h"
#include "shi/library.h"
#include "shi/sloc.h"
#include "shi/vector.h"

struct sh_operation;
struct sh_stack;

struct sh_vm {
  struct sh_vector code;
  struct sh_library core_library;
  struct sh_library *library;
  struct sh_malloc *malloc;
  struct sh_vector operations;
  struct sh_library user_library;
};

void sh_vm_init(struct sh_vm *vm, struct sh_malloc *malloc);
void sh_vm_deinit(struct sh_vm *vm);

size_t sh_emit(struct sh_vm *vm,
	       const struct sh_operation *op,
	       const void *data);

size_t sh_emit_pc(struct sh_vm *vm);

void sh_evaluate(struct sh_vm *vm,
		 struct sh_stack *stack,
		 size_t start_pc, size_t end_pc);

#endif
