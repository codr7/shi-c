#include "shi/cell.h"
#include "shi/stack.h"
#include "shi/stream.h"

struct sh_stack *sh_stack_init(struct sh_stack *s, struct sh_malloc *malloc) {
  sh_vector_init(&s->items, malloc, sizeof(struct sh_cell));
  return s;
}

void sh_stack_deinit(struct sh_stack *s) {
  sh_vector_do(&s->items, v) {
    sh_cell_deinit(v);
  }
  
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

void sh_stack_dump(struct sh_stack *s, struct sh_stream *out) {
  sh_putc(out, '[');

  for (size_t i = 0; i < s->items.length; i++) {
    if (i > 0) {
      sh_putc(out, ' ');
    }
    
    sh_cell_dump((struct sh_cell *)sh_vector_get(&s->items, i), out);
  }

  sh_putc(out, ']');
}

