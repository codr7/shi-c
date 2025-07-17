#include <stdio.h>

#include "shi/cell.h"
#include "shi/library.h"
#include "shi/libraries/core.h"
#include "shi/stack.h"
#include "shi/stream.h"

static void add_imp(struct sh_vm *vm,
		    struct sh_stack *stack,
		    const struct sh_sloc *sloc) {
  struct sh_cell *y = sh_pop(stack);
  struct sh_cell *x = sh_peek(stack);
  x->as_int += y->as_int;
  sh_cell_deinit(y);
}

static void eq_imp(struct sh_vm *vm,
		   struct sh_stack *stack,
		   const struct sh_sloc *sloc) {
  struct sh_cell *y = sh_pop(stack);
  struct sh_cell *x = sh_peek(stack);
  const bool result = sh_eq(x, y);
  sh_cell_deinit(x);
  sh_cell_init(x, SH_BOOL())->as_bool = result;
  sh_cell_deinit(y);
}

static void gt_imp(struct sh_vm *vm,
		   struct sh_stack *stack,
		   const struct sh_sloc *sloc) {
  struct sh_cell *y = sh_pop(stack);
  struct sh_cell *x = sh_peek(stack);
  x->type = SH_BOOL();
  x->as_bool = x->as_int > y->as_int;
  sh_cell_deinit(y);
}

static void lt_imp(struct sh_vm *vm,
		   struct sh_stack *stack,
		   const struct sh_sloc *sloc) {
  struct sh_cell *y = sh_pop(stack);
  struct sh_cell *x = sh_peek(stack);
  x->type = SH_BOOL();
  x->as_bool = x->as_int < y->as_int;
  sh_cell_deinit(y);
}

static void mul_imp(struct sh_vm *vm,
		    struct sh_stack *stack,
		    const struct sh_sloc *sloc) {
  struct sh_cell *y = sh_pop(stack);
  struct sh_cell *x = sh_peek(stack);
  x->as_int *= y->as_int;
  sh_cell_deinit(y);
}

static void say_imp(struct sh_vm *vm,
		    struct sh_stack *stack,
		    const struct sh_sloc *sloc) {
  struct sh_cell *v = sh_pop(stack);
  sh_cell_dump(v, sh_stdout());
  sh_cell_deinit(v);
}

static void sub_imp(struct sh_vm *vm,
		    struct sh_stack *stack,
		    const struct sh_sloc *sloc) {
  struct sh_cell *y = sh_pop(stack);
  struct sh_cell *x = sh_peek(stack);
  x->as_int -= y->as_int;
  sh_cell_deinit(y);
}

void sh_core_library_init(struct sh_library *lib, struct sh_vm *vm) {
  sh_library_init(lib, vm, "core", NULL);
  sh_bind_type(lib, SH_ANY());
  sh_bind_type(lib, SH_BOOL());
  sh_bind_type(lib, SH_INT());
  sh_bind_type(lib, SH_MACRO());
  sh_bind_type(lib, SH_META());
  sh_bind_type(lib, SH_METHOD());
  
  sh_bind(lib, "T", SH_BOOL())->as_bool = true;
  sh_bind(lib, "F", SH_BOOL())->as_bool = false;

  sh_bind_method(lib, "+", 2,
		 (struct sh_argument[]) {
		   sh_argument("x", SH_INT()),
		   sh_argument("y", SH_INT())
		 }, add_imp);

  sh_bind_method(lib, "-", 2,
		 (struct sh_argument[]) {
		   sh_argument("x", SH_INT()),
		   sh_argument("y", SH_INT())
		 }, sub_imp);

  sh_bind_method(lib, "*", 2,
		 (struct sh_argument[]) {
		   sh_argument("x", SH_INT()),
		   sh_argument("y", SH_INT())
		 }, mul_imp);

  sh_bind_method(lib, "=", 2,
		 (struct sh_argument[]) {
		   sh_argument("x", SH_INT()),
		   sh_argument("y", SH_INT())
		 }, eq_imp);

  sh_bind_method(lib, "<", 2,
		 (struct sh_argument[]) {
		   sh_argument("x", SH_INT()),
		   sh_argument("y", SH_INT())
		 }, lt_imp);

  sh_bind_method(lib, ">", 2,
		 (struct sh_argument[]) {
		   sh_argument("x", SH_INT()),
		   sh_argument("y", SH_INT())
		 }, gt_imp);

  sh_bind_method(lib, "say", 1,
		 (struct sh_argument[]) {
		   sh_argument("what", SH_ANY()),
		 }, say_imp);
}
