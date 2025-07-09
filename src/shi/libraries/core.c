#include "shi/library.h"
#include "shi/libraries/core.h"

void sh_core_library_init(struct sh_library *lib, struct sh_vm *vm) {
  sh_library_init(lib, vm, "core", NULL);
}
