#ifndef SHI_SET
#define SHI_SET

#include <stdbool.h>

#include "shi/compare.h"
#include "shi/vector.h"

struct sh_malloc;

typedef const void * (*sh_set_key_t)(const void *);

struct sh_set {
  struct sh_vector items;
  sh_compare_t compare;
  sh_set_key_t key;
};

struct sh_set *sh_set_init(struct sh_set *s,
			   struct sh_malloc *malloc,
			   size_t item_size,
			   sh_compare_t compare);

void sh_set_deinit(struct sh_set *s);
size_t sh_set_index(const struct sh_set *s, const void *key, bool *ok);
size_t sh_set_length(const struct sh_set *s);
void *sh_set_find(struct sh_set *s, const void *key);
void *sh_set_add(struct sh_set *s, const void *key, bool force);
void sh_set_clear(struct sh_set *s);

#endif
