#include "shi/cell.h"
#include "shi/libraries/core/method.h"
#include "shi/method.h"
#include "shi/stream.h"

static void method_dump(const struct sh_cell *v, struct sh_stream *out) {
  struct sh_method *m = v->as_pointer;
  sh_printf(out, "Method %s", m->name);
}

const struct sh_type SH_METHOD = {
  .name = "Method",
  SH_TYPE_DEFAULTS,
  .dump = method_dump
};
