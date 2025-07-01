#ifndef SHI_MALLOC_H
#define SHI_MALLOC_H

#include <stddef.h>
#include <stdint.h>

#define __sh_malloc_do(m, _pm, _done)		\
  bool _done = false;				\
  for (struct sh_malloc *_pm = sh_mallocp;	\
       !_done && (sh_mallocp = (m));		\
       sh_mallocp = _pm, _done = true)

#define _sh_malloc_do(m)						\
  __sh_malloc_do(m, sh_unique(malloc_pm), sh_unique(malloc_done))

#define sh_malloc_do(m)				\
  _sh_malloc_do(&(m)->malloc)

#define __sh_acquire(m, _m, s) ({		\
      struct sh_malloc *_m = m;			\
      _m->acquire(_m, s);			\
    })

#define _sh_acquire(m, s)			\
  __sh_acquire(m, sh_unique(malloc_m), s)

#define sh_acquire(s)				\
  _sh_acquire(sh_malloc(), s)

#define __sh_release(m, _m, p) do {		\
  struct sh_malloc *_m = m;			\
  _m->release(_m, p);				\
} while (0)
    
#define _sh_release(m, p)			\
  __sh_release(m, sh_unique(malloc_m), p)

#define sh_release(p)				\
  _sh_release(sh_malloc(), p)


struct sh_malloc {
  void *(*acquire)(struct sh_malloc *, size_t);
  void (*release)(struct sh_malloc *, void *);
};

extern __thread struct sh_malloc *sh_mallocp;
struct sh_malloc *sh_malloc();

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
