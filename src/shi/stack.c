#include "shi/cell.h"
#include "shi/stack.h"

struct sh_stack *sh_stack_init(struct sh_stack *s, struct sh_malloc *malloc) {
  sh_vector_init(&s->items, malloc, sizeof(struct sh_cell));
  return s;
}

void sh_stack_deinit(struct sh_stack *s) {
  sh_vector_deinit(&s->items);
}

struct sh_cell *sh_push(struct sh_stack *s) {
  return sh_vector_push(&s->items);
}

struct sh_cell *sh_peek(struct sh_stack *s) {
  return sh_vector_peek(&s->items);
}

struct sh_cell *sh_pop(struct sh_stack *s) {
  return sh_vector_pop(&s->items);
}

