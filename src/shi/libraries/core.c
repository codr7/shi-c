#include "shi/cell.h"
#include "shi/library.h"
#include "shi/libraries/core.h"

void sh_core_library_init(struct sh_library *lib, struct sh_vm *vm) {
  sh_library_init(lib, vm, "core", NULL);

  sh_bind_type(lib, SH_BOOL());
  sh_bind_type(lib, SH_INT());
  sh_bind_type(lib, SH_META());
  sh_bind_type(lib, SH_METHOD());
  
  sh_bind(lib, "T", SH_BOOL())->as_bool = true;
  sh_bind(lib, "F", SH_BOOL())->as_bool = false;
}
