#ifndef SHI_OPERATIONS_BRANCH
#define SHI_OPERATIONS_BRANCH

#include "shi/operation.h"
#include "shi/sloc.h"

extern const struct sh_operation SH_BRANCH;

struct sh_branch {
  struct sh_label *end;
};

#endif
