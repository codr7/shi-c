#ifndef SHI_STACK
#define SHI_STACK

#include "shi/vector.h"

struct sh_malloc;
typedef struct sh_vector sh_stack_t;

sh_stack_t *sh_stack_init(sh_stack_t *s, struct sh_malloc *malloc);
void sh_stack_deinit(sh_stack_t *s);
struct sh_cell *sh_stack_push(sh_stack_t *s);
struct sh_cell *sh_stack_peek(sh_stack_t *s);
struct sh_cell *sh_stack_pop(sh_stack_t *s);

#endif
