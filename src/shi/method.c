#include <assert.h>
#include <string.h>

#include "shi/call.h"
#include "shi/error.h"
#include "shi/library.h"
#include "shi/malloc.h"
#include "shi/method.h"
#include "shi/vm.h"

struct sh_argument sh_argument(const char *name, struct sh_type *type) {
  struct sh_argument a;
  strcpy(a.name, name);
  a.type = type;
  return a;
}

struct sh_method *sh_method_init(struct sh_method *m,
				 struct sh_library *library,
				 const char *name,
				 int arity,
				 struct sh_argument arguments[]) {
  m->library = library;
  strcpy(m->name, name);
  m->call = NULL;
  m->arity = arity;
  const size_t s = sizeof(struct sh_argument)*arity;
  m->arguments = sh_acquire(library->vm->malloc, s);
  memcpy(m->arguments, arguments, s);
  m->reference_count = 1;
  return m;
}

struct sh_method *sh_method_acquire(struct sh_method *m) {
  m->reference_count++;
  return m;
}

void sh_method_call(struct sh_method *m,
		    size_t *pc,
		    struct sh_stack *stack,
		    struct sh_sloc *sloc) {
  assert(m->call);
  m->call(m, pc, stack, sloc);
}

void sh_method_release(struct sh_method *m) {
  if (!m->reference_count) {
    sh_throw("Method already released");
  }
  
  if (!--m->reference_count) {
    sh_release(m->library->vm->malloc, m->arguments);    
    sh_release(m->library->vm->malloc, m);    
  }
}

static void c_call(struct sh_method *m,
		   size_t *pc,
		   struct sh_stack *stack,
		   struct sh_sloc *sloc) {
  struct sh_c_method *cm = sh_baseof(m, struct sh_c_method, method);
  cm->body(m->library->vm, stack, sloc);
}

struct sh_c_method *sh_c_method_init(struct sh_c_method *m,
				     struct sh_library *library,
				     const char *name,
				     int arity,
				     struct sh_argument arguments[],
				     sh_method_body_t body) {
  
  sh_method_init(&m->method, library, name, arity, arguments);
  m->method.call = c_call;
  m->body = body;
  return m;  
}

static void shi_call(struct sh_method *_m,
		   size_t *pc,
		   struct sh_stack *stack,
		   struct sh_sloc *sloc) {
  struct sh_shi_method *m = sh_baseof(_m, struct sh_shi_method, method);
  sh_call(m, sloc, *pc);
  *pc = m->start_pc;
}

struct sh_shi_method *sh_shi_method_init(struct sh_shi_method *m,
					 struct sh_library *library,
					 const char *name,
					 int arity,
					 struct sh_argument arguments[],
					 size_t r_arguments,
					 size_t start_pc) {
  
  sh_method_init(&m->method, library, name, arity, arguments);
  m->method.call = shi_call;
  m->r_arguments = r_arguments;
  m->start_pc = start_pc;
  return m;  
}
