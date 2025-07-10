#include <ctype.h>
#include <inttypes.h>

#include "shi/error.h"
#include "shi/form.h"
#include "shi/forms/identifier.h"
#include "shi/forms/literal.h"
#include "shi/read.h"
#include "shi/list.h"
#include "shi/malloc.h"
#include "shi/sloc.h"
#include "shi/stream.h"
#include "shi/vm.h"

bool sh_read_form(struct sh_vm *vm,
		  const char **in,
		  struct sh_list *out,
		  struct sh_sloc *sloc) {
  sh_read_whitespace(in, sloc);
  const char c = **in;
  if (!c) { return false; }

  switch (c) {
  case '(':
    sh_throw("Not implemented!");
    break;
  default:
    if (isdigit(c)) {
      return sh_read_int(vm, in, out, sloc);
    }

    return sh_read_identifier(vm, in, out, sloc);
  }
  
  sh_throw("Invalid syntax: %c", c);     
  return false;
}

bool sh_read_forms(struct sh_vm *vm,
		   const char **in,
		   struct sh_list *out,
		   struct sh_sloc *sloc) {
  bool result = false;
  
  while (sh_read_form(vm, in, out, sloc)) {
    result = true;
  }

  return result;
}

bool sh_read_identifier(struct sh_vm *vm,
			const char **in,
			struct sh_list *out,
			struct sh_sloc *sloc) {
  struct sh_sloc floc = *sloc;
  struct sh_memory_stream buffer;
  sh_memory_stream_init(&buffer, vm->malloc);
  sh_defer(sh_stream_deinit(&buffer.stream));
  char c = 0;

  while ((c = **in)) {
    if (isspace(c) || c == '(' || c == ')') {
      break;
    }
  
    sh_putc(&buffer.stream, c);
    sh_sloc_step(sloc, c);
    (*in)++;
  }

  if (!buffer.data.length) {
    return false;
  }
  
  struct sh_identifier *f = sh_acquire(vm->malloc, sizeof(struct sh_identifier));
  sh_identifier_init(f, vm, floc, out, sh_memory_stream_string(&buffer));
  return true;
}

bool sh_read_int(struct sh_vm *vm,
		 const char **in,
		 struct sh_list *out,
		 struct sh_sloc *sloc) {
  struct sh_sloc floc = *sloc;
  int64_t v = 0;
  char c = 0;
  const char *start = *in;
  
  while ((c = **in)) {    
    if (!isdigit(c)) {
      break;
    }
  
    v = v * 10 + c - '0';
    sh_sloc_step(sloc, c);
    (*in)++;
  }

  if (*in == start) {
    return false;
  }

  struct sh_literal *f = sh_acquire(vm->malloc, sizeof(struct sh_literal));
  sh_literal_init(f, vm, floc, out);
  sh_cell_init(&f->value, &SH_INT)->as_int = v;
  return true;
}

void sh_read_whitespace(const char **in, struct sh_sloc *sloc) {
  for (;isspace(**in); (*in)++) {
    sh_sloc_step(sloc, **in);
  }
}

