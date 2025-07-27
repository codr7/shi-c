#ifndef SHI_OPERATIONS_SET_REGISTER
#define SHI_OPERATIONS_SET_REGISTER

#include "shi/operation.h"
#include "shi/sloc.h"

extern const struct sh_operation SH_SET_REGISTER;

struct sh_set_register {
  size_t r_target;
  int count;
};

#endif
