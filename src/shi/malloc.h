#ifndef SHI_MALLOC_H
#define SHI_MALLOC_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#define _sh_acquire(m, _m, s) ({		\
      struct sh_malloc *_m = m;			\
      assert(_m->acquire);			\
      _m->acquire(_m, s);			\
    })

#define sh_acquire(m, s)			\
  _sh_acquire(m, sh_unique(_m), s)

#define _sh_release(m, _m, p) ({		\
      struct sh_malloc *_m = m;			\
      assert(_m->release);			\
      _m->release(_m, p);			\
    })

#define sh_release(m, p)			\
  _sh_release(m, sh_unique(_m), p)

struct sh_malloc {
  void *(*acquire)(struct sh_malloc *, size_t);
  void (*release)(struct sh_malloc *, void *);
};

extern struct sh_malloc sh_malloc_default;

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
