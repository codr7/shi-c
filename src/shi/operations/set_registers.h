#ifndef SHI_OPERATIONS_SET_REGISTERS
#define SHI_OPERATIONS_SET_REGISTERS

#include "shi/operation.h"
#include "shi/sloc.h"

extern const struct sh_operation SH_SET_REGISTERS;

struct sh_set_registers {
  size_t r_target;
  int count;
};

#endif
