#include "shi/cell.h"
#include "shi/libraries/core.h"
#include "shi/method.h"
#include "shi/stream.h"
#include "shi/type.h"

static void method_copy(struct sh_cell *dst, struct sh_cell *src, struct sh_vm *vm) {
  dst->as_other = sh_method_acquire(src->as_other);
}

static void method_deinit(struct sh_cell *v) {
  sh_method_release(v->as_other);
}

static void method_write(const struct sh_cell *v, struct sh_stream *out) {
  struct sh_method *m = v->as_other;
  sh_printf(out, "Method %s", m->name);
}

struct sh_type *SH_METHOD() {
  static __thread struct sh_type t = {
    .name = "Method",
    SH_TYPE_DEFAULTS,
    .copy = method_copy,
    .deinit = method_deinit,
    .write = method_write
  };
  
  return &t;
}
