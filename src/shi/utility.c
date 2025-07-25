#include <string.h>

#include "shi/malloc.h"
#include "shi/utility.h"

size_t sh_alignof(size_t size) {
  const size_t max = _Alignof(max_align_t);
  if (size >= max) { return max; }
  size_t v = 1;
  for (size_t nv = 1; nv <= size; v = nv, nv = v << 1);
  return v;
}

enum sh_order sh_strcmp(const char *x, const char *y) {
  const int result = strcmp(x, y);
  if (!result) { return SH_EQ; }
  return (result < 0) ? SH_LT : SH_GT;
}

char *sh_strdup(const char *in, struct sh_malloc *malloc) {
  const size_t n = strlen(in);
  char *out = sh_acquire(malloc, n+1);
  strcpy(out, in);
  return out;
}
