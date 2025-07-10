#include "shi/cell.h"
#include "shi/libraries/core.h"
#include "shi/method.h"
#include "shi/stream.h"
#include "shi/type.h"

static void method_write(const struct sh_cell *v, struct sh_stream *out) {
  struct sh_method *m = v->as_other;
  sh_printf(out, "Method %s", m->name);
}

struct sh_type *SH_METHOD() {
  static __thread struct sh_type t = {
    .name = "Method",
    SH_TYPE_DEFAULTS,
    .write = method_write
  };
  
  return &t;
}
