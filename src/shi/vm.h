#ifndef SHI_VM
#define SHI_VM

#include "shi/cell.h"
#include "shi/sloc.h"
#include "shi/vector.h"

struct sh_vm {
  struct sh_vector operations;
  struct sh_vector code;
};

void sh_vm_init(struct sh_vm *vm, struct sh_malloc *malloc);
void sh_vm_deinit(struct sh_vm *vm);

struct sh_operation;

size_t sh_emit(struct sh_vm *vm,
	       const struct sh_operation *op,
	       const void *data);

void sh_evaluate(struct sh_vm *vm,
		 sh_stack_t *stack,
		 size_t start_pc, size_t end_pc);

extern const struct sh_type SH_VM_FUN;
typedef void (*sh_vm_fun_t)(struct sh_vm *, struct sh_sloc);

typedef uint8_t *(*sh_evaluate_t)(struct sh_vm *, sh_stack_t *stack, uint8_t *);

struct sh_operation {
  const char *name;

  size_t align;
  size_t size;

  sh_evaluate_t eval;
  void (*deinit)(uint8_t *);
};

struct sh_call_operation {
  sh_vm_fun_t target;
  struct sh_sloc sloc;
};

extern const struct sh_operation SH_CALL;

struct sh_push_operation {
  struct sh_cell value;
};

extern const struct sh_operation SH_PUSH;

#endif
