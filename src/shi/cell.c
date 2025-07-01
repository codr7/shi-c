#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "cell.h"
#include "stream.h"

struct sh_cell *sh_cell_init(struct sh_cell *v, const struct sh_type *t) {
  v->type = t;
  return v;
}

void sh_cell_deinit(struct sh_cell *v) {
  if (v->type->deinit) {
    v->type->deinit(v);
  }
}

struct sh_cell *sh_cell_copy(struct sh_cell *dst, struct sh_cell *src) {
  const struct sh_type *t = src->type;
  
  if (t->copy) {
    dst->type = t;
    t->copy(dst, src);
  } else {
    *dst = *src;
  }

  return dst;
}

void sh_cell_dump(struct sh_cell *v, struct sh_stream *out) {
  if (v->type->dump) {
    v->type->dump(v, out);
  } else {
    sh_cell_write(v, out);
  }
}

void sh_cell_write(struct sh_cell *v, struct sh_stream *out) {
  assert(v->type->write);
  v->type->write(v, out);
}

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
