#ifndef SHI_OPERATIONS_GET_REGISTERS
#define SHI_OPERATIONS_GET_REGISTERS

#include "shi/operation.h"
#include "shi/sloc.h"

extern const struct sh_operation SH_GET_REGISTERS;

struct sh_get_registers {
  size_t r_source;
  int count;
};

#endif
