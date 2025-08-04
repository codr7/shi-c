#ifndef SHI_OPERATION
#define SHI_OPERATION

#include <stddef.h>
#include <stdint.h>

struct sh_cell;
struct sh_label;
struct sh_method;
struct sh_sloc;
struct sh_stack;
struct sh_vm;

typedef uint8_t *(*sh_evaluate_t)(struct sh_vm *, struct sh_stack *, uint8_t *);

struct sh_operation {
  char name[64];

  size_t align;
  size_t size;

  void (*deinit)(uint8_t *);
  sh_evaluate_t evaluate;
};

struct sh_operation *sh_operation_init(struct sh_operation *op,
				       const char *name,
				       size_t size,
				       size_t align);
  
void sh_emit_benchmark(struct sh_vm *vm,
		       int rounds,
		       struct sh_label *end);

void sh_emit_branch(struct sh_vm *vm, struct sh_label *end);

void sh_emit_call_method(struct sh_vm *vm,
			 struct sh_method *target,
			 struct sh_sloc *sloc);

void sh_emit_check_value(struct sh_vm *vm,
			 struct sh_cell *expected,
			 struct sh_sloc sloc);

void sh_emit_get_registers(struct sh_vm *vm,
			   const size_t r_source,
			   const int count);

#endif
