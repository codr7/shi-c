#ifndef SHI_OPERATIONS_CALL_METHOD
#define SHI_OPERATIONS_CALL_METHOD

#include "shi/operation.h"
#include "shi/sloc.h"

struct sh_method;

extern const struct sh_operation SH_CALL_METHOD;

struct sh_call_method {
  struct sh_method *target;
  struct sh_sloc sloc;
};

#endif
