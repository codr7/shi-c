#include <stdio.h>
#include <string.h>

#include "shi/cell.h"
#include "shi/error.h"
#include "shi/form.h"
#include "shi/forms/identifier.h"
#include "shi/library.h"
#include "shi/libraries/core.h"
#include "shi/operations/branch.h"
#include "shi/operations/check_value.h"
#include "shi/operations/goto.h"
#include "shi/operations/return.h"
#include "shi/stack.h"
#include "shi/stream.h"
#include "shi/vm.h"

static void add_imp(struct sh_vm *vm,
		    struct sh_stack *stack,
		    struct sh_sloc *sloc) {
  struct sh_cell *y = sh_pop(stack);
  struct sh_cell *x = sh_peek(stack);
  x->as_int += y->as_int;
  sh_cell_deinit(y);
}

static void check_imp(struct sh_vm *vm,
		      struct sh_sloc *sloc,
		      struct sh_list *arguments) {
  struct sh_form *expected = sh_baseof(sh_list_pop_front(arguments),
				struct sh_form,
				owner);
  
  sh_defer(sh_form_release(expected, vm));
  struct sh_cell *ev = sh_form_value(expected, vm);

  if (!ev) {
    sh_throw("Error in %s: Missing expected value", sh_sloc_string(sloc));
  }
  
  struct sh_form *actual = sh_baseof(sh_list_pop_front(arguments),
			      struct sh_form,
			      owner);
  sh_defer(sh_form_release(actual, vm));
  sh_form_emit(actual, vm, arguments);
  
  struct sh_check_value op;
  op.sloc = *sloc;
  sh_cell_copy(&op.expected, ev, vm);
  sh_emit(vm, &SH_CHECK_VALUE, &op); 
}

static void eq_imp(struct sh_vm *vm,
		   struct sh_stack *stack,
		   struct sh_sloc *sloc) {
  struct sh_cell *y = sh_pop(stack);
  struct sh_cell *x = sh_peek(stack);
  const bool result = sh_eq(x, y);
  sh_cell_deinit(x);
  sh_cell_init(x, SH_BOOL())->as_bool = result;
  sh_cell_deinit(y);
}

static void gt_imp(struct sh_vm *vm,
		   struct sh_stack *stack,
		   struct sh_sloc *sloc) {
  struct sh_cell *y = sh_pop(stack);
  struct sh_cell *x = sh_peek(stack);
  x->type = SH_BOOL();
  x->as_bool = x->as_int > y->as_int;
  sh_cell_deinit(y);
}

static void if_imp(struct sh_vm *vm,
		   struct sh_sloc *sloc,
		   struct sh_list *arguments) {
  struct sh_form *c = sh_baseof(sh_list_pop_front(arguments),
				struct sh_form,
				owner);
  sh_defer(sh_form_release(c, vm));
  sh_form_emit(c, vm, arguments);
  
  struct sh_label *end = sh_label(vm);
  sh_emit(vm, &SH_BRANCH, &(struct sh_branch){.end = end}); 

  struct sh_form *l = sh_baseof(sh_list_pop_front(arguments),
				struct sh_form,
				owner);
  sh_defer(sh_form_release(l, vm));
  sh_form_emit(l, vm, arguments);

  if (arguments->next != arguments) {
    struct sh_form *next = sh_baseof(sh_list_peek_front(arguments),
				     struct sh_form,
				     owner);
    
    if (next->type == &SH_IDENTIFIER) {
      struct sh_identifier *id = sh_baseof(next, struct sh_identifier, form);
      
      if (strcmp("else", id->name) == 0) {
	sh_list_delete(&next->owner);
	sh_defer(sh_form_release(next, vm));

	struct sh_label *rend = sh_label(vm);
	sh_emit(vm, &SH_GOTO, &(struct sh_goto){.target = rend}); 
	end->pc = sh_emit_pc(vm);
	
	struct sh_form *r = sh_baseof(sh_list_pop_front(arguments),
				      struct sh_form,
				      owner);
	sh_defer(sh_form_release(r, vm));
	sh_form_emit(r, vm, arguments);
	rend->pc = sh_emit_pc(vm);
	return;
      }
    }
  }

  end->pc = sh_emit_pc(vm);
}

static void lt_imp(struct sh_vm *vm,
		   struct sh_stack *stack,
		   struct sh_sloc *sloc) {
  struct sh_cell *y = sh_pop(stack);
  struct sh_cell *x = sh_peek(stack);
  x->type = SH_BOOL();
  x->as_bool = x->as_int < y->as_int;
  sh_cell_deinit(y);
}

static void method_imp(struct sh_vm *vm,
		       struct sh_sloc *sloc,
		       struct sh_list *arguments) {
  struct sh_form *fname = sh_baseof(sh_list_pop_front(arguments),
				    struct sh_form,
				    owner);
  sh_defer(sh_form_release(fname, vm));
  
  if (fname->type != &SH_IDENTIFIER) {
    sh_throw("Error in %s: Expected method name", sh_sloc_string(sloc));
  }

  const char *name = sh_baseof(fname, struct sh_identifier, form)->name;

  struct sh_form *fargs = sh_baseof(sh_list_pop_front(arguments),
				    struct sh_form,
				    owner);
  sh_defer(sh_form_release(fargs, vm));
  
  struct sh_form *body = sh_baseof(sh_list_pop_front(arguments),
				   struct sh_form,
				   owner);
  sh_defer(sh_form_release(body, vm));

  struct sh_label *skip = sh_label(vm);
  sh_emit(vm, &SH_GOTO, &(struct sh_goto){.target = skip}); 
  sh_form_emit(body, vm, arguments);
  sh_emit(vm, &SH_RETURN, NULL);
  skip->pc = sh_emit_pc(vm);
}

static void mul_imp(struct sh_vm *vm,
		    struct sh_stack *stack,
		    struct sh_sloc *sloc) {
  struct sh_cell *y = sh_pop(stack);
  struct sh_cell *x = sh_peek(stack);
  x->as_int *= y->as_int;
  sh_cell_deinit(y);
}

static void say_imp(struct sh_vm *vm,
		    struct sh_stack *stack,
		    struct sh_sloc *sloc) {
  struct sh_cell *v = sh_pop(stack);
  sh_cell_dump(v, sh_stdout());
  sh_cell_deinit(v);
}

static void sub_imp(struct sh_vm *vm,
		    struct sh_stack *stack,
		    struct sh_sloc *sloc) {
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

  sh_bind_macro(lib, "check", 2,
		sh_macro_arguments("expected", "actual"),
		check_imp);

  sh_bind_macro(lib, "if", 2,
		sh_macro_arguments("cond", "left"),
		if_imp);

  sh_bind_macro(lib, "method", 3,
		sh_macro_arguments("name", "args", "body"),
		method_imp);

  sh_bind_method(lib, "say", 1,
		 (struct sh_argument[]) {
		   sh_argument("what", SH_ANY()),
		 }, say_imp);
}
