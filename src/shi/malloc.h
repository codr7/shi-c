#ifndef SHI_MALLOC_H
#define SHI_MALLOC_H

#include <stddef.h>
#include <stdint.h>

struct sh_malloc {
  void *(*acquire)(struct sh_malloc *, size_t);
  void (*release)(struct sh_malloc *, void *);
};

extern struct sh_malloc sh_malloc_default;

void *sh_acquire(struct sh_malloc *malloc, size_t size);
void sh_release(struct sh_malloc *malloc, void *pointer);

/* Bump */

struct sh_bump_alloc {
  struct sh_malloc malloc;
  struct sh_malloc *source;
  size_t size, offset;
  uint8_t *memory;
};

void sh_bump_alloc_init(struct sh_bump_alloc *a,
			struct sh_malloc *source,
			size_t size);

void sh_bump_alloc_deinit(struct sh_bump_alloc *a);

#endif
