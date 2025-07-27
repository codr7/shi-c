#ifndef SHI_OPERATIONS_GET_REGISTER
#define SHI_OPERATIONS_GET_REGISTER

#include "shi/operation.h"
#include "shi/sloc.h"

extern const struct sh_operation SH_GET_REGISTER;

struct sh_get_register {
  size_t r_source;
  int count;
};

#endif
