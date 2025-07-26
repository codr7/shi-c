#ifndef SHI_VECTOR
#define SHI_VECTOR

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "utility.h"

#define _sh_vector_do(v, _v, var)			\
  struct sh_vector *_v = v;				\
  for (void *var = _v->start;				\
       var < (void *)_v->end;				\
       var += _v->item_size)

#define sh_vector_do(v, var)				\
  _sh_vector_do(v, sh_unique(vector), var)

struct sh_malloc;

struct sh_vector {
  size_t item_size, capacity, length;
  uint8_t *start, *end;
  struct sh_malloc *malloc;
};

struct sh_vector *sh_vector_init(struct sh_vector *v,
				 struct sh_malloc *malloc,
				 size_t item_size);

void sh_vector_deinit(struct sh_vector *v);
void sh_vector_grow(struct sh_vector *v, size_t capacity);
void sh_vector_clear(struct sh_vector *v);
void *sh_vector_get(struct sh_vector *v, size_t i);
const void *sh_vector_get_const(const struct sh_vector *v, size_t i);
void *sh_vector_push(struct sh_vector *v);
void *sh_vector_peek(struct sh_vector *v);
void *sh_vector_pop(struct sh_vector *v);
void *sh_vector_insert(struct sh_vector *v, size_t i, size_t n);
bool sh_vector_delete(struct sh_vector *v, size_t i, size_t n);

#endif

