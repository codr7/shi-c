#ifndef SHI_OPERATIONS_CHECK_VALUE
#define SHI_OPERATIONS_CHECK_VALUE

#include "shi/cell.h"
#include "shi/operation.h"
#include "shi/sloc.h"

extern const struct sh_operation SH_CHECK_VALUE;

struct sh_check_value {
  struct sh_cell expected;
  struct sh_sloc sloc;
};

#endif
