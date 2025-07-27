#ifndef SHI_VM
#define SHI_VM

#include <stdio.h>

#include "shi/cell.h"
#include "shi/library.h"
#include "shi/list.h"
#include "shi/sloc.h"
#include "shi/vector.h"

#define _sh_library_do(vm, _vm, _prev, _lib)				\
  struct sh_vm *_vm = vm;						\
  struct sh_library _lib;						\
  sh_library_init(&_lib, _vm, _vm->library->name, _vm->library);	\
  for (struct sh_library *_prev = _vm->library;				\
       _prev && (vm->library = &_lib);					\
       _vm->library = _prev, _prev = NULL, sh_library_deinit(&_lib))

#define sh_library_do(vm)						\
  _sh_library_do(vm, sh_unique(v), sh_unique(p), sh_unique(l))

struct sh_operation;
struct sh_stack;

struct sh_label {
  struct sh_list owner;
  size_t pc;
};

struct sh_vm {
  struct sh_call *call_cache;
  struct sh_call *call_stack;
  struct sh_vector code;
  struct sh_library core_library;
  struct sh_list labels;
  struct sh_library *library;
  struct sh_malloc *malloc;
  struct sh_vector operations;
  struct sh_vector registers;
  struct sh_library user_library;
};

void sh_vm_init(struct sh_vm *vm, struct sh_malloc *malloc);
void sh_vm_deinit(struct sh_vm *vm);

size_t sh_allocate_registers(struct sh_vm *vm, size_t n);

size_t sh_emit(struct sh_vm *vm,
	       const struct sh_operation *op,
	       const void *data);

size_t sh_emit_pc(struct sh_vm *vm);

void sh_evaluate(struct sh_vm *vm,
		 struct sh_stack *stack,
		 size_t start, size_t end);

struct sh_label *sh_label(struct sh_vm *vm);

uint8_t *sh_pc_pointer(struct sh_vm *vm, size_t pc);
size_t sh_pointer_pc(struct sh_vm *vm, const uint8_t *p);
struct sh_cell *sh_register(struct sh_vm *vm, size_t r_target);
void sh_shell(struct sh_vm *vm, FILE *in, FILE *out);

#endif
