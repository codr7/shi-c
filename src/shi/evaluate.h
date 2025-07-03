#ifndef SHI_EVALUATE
#define SHI_EVALUATE

#include "shi/cell.h"
#include "shi/macro.h"
#include "shi/operation.h"
#include "shi/sloc.h"

struct sh_call_operation {
  struct sh_method *target;
  struct sh_sloc sloc;
};

extern const struct sh_operation SH_CALL;

struct sh_push_operation {
  struct sh_cell value;
};

extern const struct sh_operation SH_PUSH;

#endif
