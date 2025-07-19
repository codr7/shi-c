#include <assert.h>
#include <string.h>

#include "shi/error.h"
#include "shi/library.h"
#include "shi/malloc.h"
#include "shi/macro.h"
#include "shi/vm.h"

struct sh_macro *sh_macro_init(struct sh_macro *m,
				 struct sh_library *library,
				 const char *name,
				 int arity,
				 sh_macro_argument_t arguments[]) {
  m->library = library;
  strcpy(m->name, name);
  m->emit = NULL;
  m->arity = arity;
  const size_t s = sizeof(sh_macro_argument_t)*arity;
  m->arguments = sh_acquire(library->vm->malloc, s);
  memcpy(m->arguments, arguments, s);
  m->reference_count = 1;
  return m;
}

struct sh_macro *sh_macro_acquire(struct sh_macro *m) {
  m->reference_count++;
  return m;
}

void sh_macro_emit(struct sh_macro *m,
		   struct sh_sloc *sloc,
		   struct sh_list *arguments) {
  assert(m->emit);

  if (sh_list_length(arguments) < m->arity) {
    sh_throw("Error in %s: Not enough arguments", sh_sloc_string(sloc));
  }
  
  m->emit(m, sloc, arguments);
}

void sh_macro_release(struct sh_macro *m) {
  if (!m->reference_count) {
    sh_throw("Macro already released");
  }
  
  if (!--m->reference_count) {
    sh_release(m->library->vm->malloc, m->arguments);    
    sh_release(m->library->vm->malloc, m);    
  }
}

static void c_emit(struct sh_macro *m,
		   struct sh_sloc *sloc,
		   struct sh_list *arguments) {
  struct sh_c_macro *cm = sh_baseof(m, struct sh_c_macro, macro);
  cm->body(m->library->vm, sloc, arguments);
}

struct sh_c_macro *sh_c_macro_init(struct sh_c_macro *m,
				     struct sh_library *library,
				     const char *name,
				     int arity,
				     sh_macro_argument_t arguments[],
				     sh_macro_body_t body) {
  
  sh_macro_init(&m->macro, library, name, arity, arguments);
  m->macro.emit = c_emit;
  m->body = body;
  return m;  
}
