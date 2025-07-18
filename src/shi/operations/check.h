#ifndef SHI_OPERATIONS_CHECK
#define SHI_OPERATIONS_CHECK

#include "shi/cell.h"
#include "shi/operation.h"
#include "shi/sloc.h"

extern const struct sh_operation SH_CHECK;

struct sh_check {
  struct sh_cell expected;
  struct sh_sloc sloc;
};

#endif
