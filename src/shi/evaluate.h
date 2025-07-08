#ifndef SHI_EVALUATE
#define SHI_EVALUATE

#include "shi/cell.h"
#include "shi/operation.h"
#include "shi/sloc.h"
#include "shi/utility.h"

extern const struct sh_operation SH_CALL_METHOD;

struct sh_call_method {
  struct sh_method *target;
  struct sh_sloc sloc;
};

extern const struct sh_operation SH_PUSH_VALUE;

struct sh_push_value {
  struct sh_cell value;
};

#endif
