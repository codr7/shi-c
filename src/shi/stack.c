#include "shi/cell.h"
#include "shi/stack.h"

sh_stack_t *sh_stack_init(sh_stack_t *s, struct sh_malloc *malloc) {
  sh_vector_init(s, malloc, sizeof(struct sh_cell));
  return s;
}

void sh_stack_deinit(sh_stack_t *s) {
  sh_vector_deinit(s);
}

struct sh_cell *sh_stack_push(sh_stack_t *s) {
  return sh_vector_push(s);
}

struct sh_cell *sh_stack_peek(sh_stack_t *s) {
  return sh_vector_peek(s);
}

struct sh_cell *sh_stack_pop(sh_stack_t *s) {
  return sh_vector_pop(s);
}

