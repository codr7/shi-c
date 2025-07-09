#ifndef SHI_UTILITY
#define SHI_UTILITY

#include <stddef.h>
#include <stdint.h>

#include "shi/compare.h"

struct sh_malloc;

#define sh_abs(x) ({				\
      __auto_type _x = x;			\
      _x < 0 ? -x : x;				\
    })						\

#define sh_align(base, size) ({				\
      __auto_type _base = base;				\
      __auto_type _size = sh_alignof(size);		\
      __auto_type _rest = (ptrdiff_t)_base % _size;	\
      (_rest) ? _base + _size - _rest : _base;		\
    })							\

size_t sh_alignof(size_t size);

#define _sh_array(t, a, n, ...)			\
  t a[] = { __VA_ARGS__ };			\
  const size_t n = sizeof(a) / sizeof(t)		

#define sh_array(t, p, ...)					\
  _sh_array(t, sh_id(p, _a), sh_id(p, _n), ##__VA_ARGS__)

#define _sh_baseof(p, t, m, _p) ({		\
      uint8_t *_p = (uint8_t *)(p);		\
      _p ? ((t *)(_p - offsetof(t, m))) : NULL;	\
    })

#define sh_baseof(p, t, m)			\
  _sh_baseof(p, t, m, sh_unique(pointer))

#define sh_bitmask(v, bc)			\
  (v & ((1 << bc) - 1))

#define sh_const(x) ({						\
      sizeof(int) ==						\
      sizeof(*(8 ? ((void *)((long)(x) * 0l)) : (int *)8))	\
    })

#define _sh_defer(_d, _v, ...)			\
  void _d(int *) { __VA_ARGS__; }		\
  int _v __attribute__ ((__cleanup__(_d)))

#define sh_defer(...)							\
  _sh_defer(sh_unique(defer_d), sh_unique(defer_v), ##__VA_ARGS__)

#define _sh_id(x, y)				\
  x ## y

#define sh_id(x, y)				\
  _sh_id(x, y)

#define sh_max(x, y) ({				\
      __auto_type _x = x;			\
      __auto_type _y = y;			\
      _x > _y ? _x : _y;			\
    })						\

#define sh_min(x, y) ({				\
      __auto_type _x = x;			\
      __auto_type _y = y;			\
      _x < _y ? _x : _y;			\
    })						\

#define sh_sign(x)				\
  (x < 0 ? -1 : 1)				\

#define sh_unique(x)				\
  sh_id(x, __COUNTER__)

enum sh_order sh_strcmp(const char *x, const char *y);

char *sh_strdup(const char *in, struct sh_malloc *malloc);

#endif
