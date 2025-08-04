#ifndef SHI_STACK
#define SHI_STACK

#include "shi/vector.h"

struct sh_malloc;
struct sh_stream;

struct sh_stack {
  struct sh_vector items;
};
  
struct sh_stack *sh_stack_init(struct sh_stack *s, struct sh_malloc *malloc);
void sh_stack_deinit(struct sh_stack *s);

struct sh_cell *sh_push(struct sh_stack *s);
struct sh_cell *sh_peek(struct sh_stack *s);
struct sh_cell *sh_pop(struct sh_stack *s);

void sh_stack_clear(struct sh_stack *s);
void sh_stack_dump(struct sh_stack *s, struct sh_stream *out);

#endif
