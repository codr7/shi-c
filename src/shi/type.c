#include "shi/cell.h"
#include "shi/evaluate.h"
#include "shi/method.h"
#include "shi/sloc.h"
#include "shi/type.h"
#include "shi/stream.h"
#include "shi/vm.h"

void sh_type_default_copy(struct sh_cell *dst, struct sh_cell *src,
			 struct sh_vm *vm) {
  *dst = *src;
}

void sh_type_default_emit(struct sh_cell *v,
			 struct sh_vm *vm,
			 struct sh_sloc sloc,
			 struct sh_list *args) {
  struct sh_push_value op;
  sh_cell_copy(&op.value, v, vm);
  sh_emit(vm, &SH_PUSH_VALUE, &op);
}
