#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "malloc.h"
#include "vector.h"

static void grow(struct sh_vector *v) {
  sh_vector_grow(v, v->capacity ? v->capacity*2 : 2);
}

struct sh_vector *sh_vector_init(struct sh_vector *v,
				 struct sh_malloc *malloc,
				 const size_t item_size) {
  v->malloc = malloc;
  v->item_size = item_size;
  v->capacity = 0;
  v->length = 0;
  v->start = v->end = NULL;
  return v;
}

void sh_vector_deinit(struct sh_vector *v) {
  if (v->start) { sh_release(v->malloc, v->start); }
}

void *sh_vector_append(struct sh_vector *v, size_t n) {
  const size_t m = v->length+n;
  if (m > v->capacity) { sh_vector_grow(v, m); } 
  uint8_t *const p = v->end;
  v->length += n;
  v->end += n*v->item_size;
  return p;
}

void sh_vector_clear(struct sh_vector *v) {
  v->length = 0;
  v->end = v->start;
}

void *sh_vector_get(struct sh_vector *v, const size_t i) {
  return v->start ? v->start + v->item_size*i : NULL;
}

const void *sh_vector_get_const(const struct sh_vector *v, const size_t i) {
  return v->start ? v->start + v->item_size*i : NULL;
}

void sh_vector_grow(struct sh_vector *v, const size_t capacity) {
  v->capacity = capacity; 
  size_t size = v->item_size * (v->capacity+1);
  uint8_t *new_start = sh_acquire(v->malloc, size);

  if (v->start) {
    memmove(new_start, v->start, v->length * v->item_size);
    sh_release(v->malloc, v->start); 
  }
  
  v->start = new_start;
  v->end = v->start + v->item_size*v->length;
}

void *sh_vector_push(struct sh_vector *v) {
  if (v->length == v->capacity) { grow(v); }
  void *p = v->end;
  v->end += v->item_size;
  v->length++;
  return p;
}

void *sh_vector_peek(struct sh_vector *v) {
  return v->length ? v->end - v->item_size : NULL;
}

void *sh_vector_pop(struct sh_vector *v) {
  if (!v->length) { return NULL; }
  v->end -= v->item_size;
  v->length--;
  return v->end;
}

void *sh_vector_insert(struct sh_vector *v, const size_t i, const size_t n) {
  const size_t m = v->length+n;
  if (m > v->capacity) { sh_vector_grow(v, m); } 
  uint8_t *const p = sh_vector_get(v, i);

  if (i < v->length) {
    memmove(p + v->item_size*n, p, (v->length - i) * v->item_size);
  }
  
  v->length += n;
  v->end += n*v->item_size;
  return p;
}

bool sh_vector_delete(struct sh_vector *v, const size_t i, const size_t n) {
  const size_t m = i+n;
  assert(m <= v->length);

  if (m < v->length) {
    uint8_t *const p = sh_vector_get(v, i);
    memmove(p, p + n*v->item_size, i + (v->length-n) * v->item_size);
  }

  v->length -= n;
  v->end -= n*v->item_size;
  return true;
}
