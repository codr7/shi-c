#include <stdlib.h>

#include "shi/set.h"

struct sh_set *sh_set_init(struct sh_set *s,
			   struct sh_malloc *malloc,
			   const size_t item_size,
			   sh_compare_t compare) {
  sh_vector_init(&s->items, malloc, item_size);
  s->compare = compare;
  s->key = NULL;
  return s;
}

void sh_set_deinit(struct sh_set *s) {
  sh_vector_deinit(&s->items);
}

size_t sh_set_index(const struct sh_set *s, const void *key, bool *ok) {
  size_t min = 0, max = s->items.length;

  while (min < max) {
    const size_t i = (min+max)/2;
    const void *v = sh_vector_get_const(&s->items, i);
    const void *k = s->key ? s->key(v) : v;

    switch (s->compare(key, k)) {
    case SH_LT:
      max = i;
      break;
    case SH_GT:
      min = i+1;
      break;
    default:
      if (ok) {
	*ok = true;
      }
      
      return i;
    }
  }

  return min;
}

size_t sh_set_length(const struct sh_set *s) {
  return s->items.length;
}

void *sh_set_find(struct sh_set *s, const void *key) {
  bool ok = false;
  const size_t i = sh_set_index(s, key, &ok);
  return ok ? sh_vector_get(&s->items, i) : NULL;
}

void *sh_set_add(struct sh_set *s, const void *key, const bool force) {
  bool ok = false;
  const size_t i = sh_set_index(s, key, &ok);

  if (ok && !force) {
    return NULL;
  }
  
  return sh_vector_insert(&s->items, i, 1);
}

void sh_set_clear(struct sh_set *s) {
  sh_vector_clear(&s->items);
}
