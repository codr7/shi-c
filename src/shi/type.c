#include "shi/cell.h"
#include "shi/evaluate.h"
#include "shi/method.h"
#include "shi/sloc.h"
#include "shi/type.h"
#include "shi/stream.h"
#include "shi/vm.h"

static void default_emit(struct sh_cell *v,
			 struct sh_vm *vm,
			 struct sh_sloc sloc) {
  struct sh_push_value op;
  sh_cell_copy(&op.value, v);
  sh_emit(vm, &SH_PUSH_VALUE, &op);
}

#define SH_TYPE_DEFAULTS			\
  .emit = default_emit

static void bool_write(const struct sh_cell *v, struct sh_stream *out) {
  sh_putc(out, v->as_bool ? 'T' : 'F');
}

const struct sh_type SH_BOOL = {
  .name = "Bool",
  SH_TYPE_DEFAULTS,
  .copy = NULL,
  .write = bool_write
};

static void int_write(const struct sh_cell *v, struct sh_stream *out) {
  sh_printf(out, "%d", v->as_int);
}

const struct sh_type SH_INT = {
  .name = "Int",
  SH_TYPE_DEFAULTS,
  .copy = NULL,
  .write = int_write
};

static void method_dump(const struct sh_cell *v, struct sh_stream *out) {
  struct sh_method *m = v->as_pointer;
  sh_printf(out, "Method %s", m->name);
}

const struct sh_type SH_METHOD = {
  .name = "Method",
  SH_TYPE_DEFAULTS,
  .copy = NULL,
  .dump = method_dump
};
