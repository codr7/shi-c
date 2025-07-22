#ifndef SHI_OPERATIONS_GOTO
#define SHI_OPERATIONS_GOTO

#include "shi/operation.h"
#include "shi/sloc.h"

extern const struct sh_operation SH_GOTO;

struct sh_goto {
  struct sh_label *target;
};

#endif
