#include <string.h>
#include "shi/operation.h"

struct sh_operation *sh_operation_init(struct sh_operation *op,
				       const char *name,
				       size_t size,
				       size_t align) {
  strcpy(op->name, name);
  op->size = size;
  op->align = align;
  op->evaluate = NULL;
  op->deinit = NULL;
  return op;
}

