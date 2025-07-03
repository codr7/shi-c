#include "shi/cell.h"
#include "shi/method.h"
#include "shi/type.h"
#include "shi/stream.h"

static void bool_write(const struct sh_cell *v, struct sh_stream *out) {
  sh_puts(out, v->as_bool ? "true" : "false");
}

const struct sh_type SH_BOOL = {
  .name = "Bool",
  .copy = NULL,
  .write = bool_write
};

static void int_write(const struct sh_cell *v, struct sh_stream *out) {
  sh_printf(out, "%d", v->as_int);
}

const struct sh_type SH_INT = {
  .name = "Int",
  .copy = NULL,
  .write = int_write
};

static void method_dump(const struct sh_cell *v, struct sh_stream *out) {
  struct sh_method *m = v->as_pointer;
  sh_printf(out, "Method %s", m->name);
}

const struct sh_type SH_METHOD = {
  .name = "Method",
  .copy = NULL,
  .dump = method_dump
};
