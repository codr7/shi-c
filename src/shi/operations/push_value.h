#ifndef SHI_OPERATIONS_PUSH_VALUE
#define SHI_OPERATIONS_PUSH_VALUE

#include "shi/cell.h"
#include "shi/operation.h"

extern const struct sh_operation SH_PUSH_VALUE;

struct sh_push_value {
  struct sh_cell value;
};

#endif
