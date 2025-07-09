#ifndef SHI_LIBRARY
#define SHI_LIBRARY

#include "shi/set.h"

struct sh_library {
  struct sh_set bindings;
  char *name;
  struct sh_library *parent;
  struct sh_vm *vm;
};

struct sh_library *sh_library_init(struct sh_library *lib,
				   struct sh_vm *vm,
				   const char *name,
				   struct sh_library *parent);

void sh_library_deinit(struct sh_library *lib);

void sh_bind(struct sh_library *lib, const char *key, struct sh_cell *value);
struct sh_cell *sh_find(struct sh_library *lib, const char *key);

#endif
