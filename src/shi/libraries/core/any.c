#include <stdlib.h>

#include "shi/libraries/core.h"
#include "shi/type.h"

struct sh_type *SH_ANY() {
  static __thread struct sh_type *t = NULL;

  if (!t) {
    t = malloc(sizeof(struct sh_type));
    sh_type_init(t, "Any");
  }

  return t;
}
