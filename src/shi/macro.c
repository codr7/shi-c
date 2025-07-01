#include "macro.h"

size_t sh_alignof(size_t size) {
  const size_t max = _Alignof(max_align_t);
  if (size >= max) { return max; }
  size_t v = 1;
  for (size_t nv = 1; nv <= size; v = nv, nv = v << 1);
  return v;
}
