#include <stdio.h>

#include "shi/cell.h"
#include "shi/compare.h"
#include "shi/error.h"
#include "shi/library.h"
#include "shi/libraries/core.h"
#include "shi/malloc.h"
#include "shi/type.h"
#include "shi/vm.h"

struct sh_library_item {
  char *key;
  struct sh_cell value;
};

static enum sh_order compare_key(const void *x, const void *y) {
  return sh_strcmp((const char *)x, (const char *)y);
}

static const void *get_key(const void *x) {
  return ((const struct sh_library_item *)x)->key;
}
struct sh_library *sh_library_init(struct sh_library *lib,
				   struct sh_vm *vm,
				   const char *name,
				   struct sh_library *parent) {
  lib->vm = vm;
  sh_set_init(&lib->bindings,
	      &sh_malloc_default,
	      sizeof(struct sh_library_item),
	      compare_key);

  lib->bindings.key = get_key;
  lib->name = sh_strdup(name, vm->malloc);
  lib->parent = parent;
  return lib;
}

void sh_library_deinit(struct sh_library *lib) {
  sh_vector_do(&lib->bindings.items, p) {
    struct sh_library_item *it = *(struct sh_library_item **)p;
    sh_release(lib->vm->malloc, it->key);
    sh_cell_deinit(&it->value);
  }
  
  sh_set_deinit(&lib->bindings);
}

struct sh_cell *sh_bind(struct sh_library *lib,
			const char *key,
			struct sh_type *type) {
  struct sh_library_item *it = sh_set_add(&lib->bindings, key, false);
  it->key = sh_strdup(key, lib->vm->malloc);
  it->value.type = type;
  return &it->value;
}

void sh_bind_type(struct sh_library *lib, struct sh_type *type) {
  sh_bind(lib, type->name, SH_META())->as_other = type;
}

struct sh_cell *sh_find(struct sh_library *lib, const char *key) {
  struct sh_library_item *it = sh_set_find(&lib->bindings, key);
  return it ? &it->value : (lib->parent ? sh_find(lib->parent, key) : NULL);
}

void sh_import(struct sh_library *lib, struct sh_library *source) {
  sh_vector_do(&source->bindings.items, _it) {
    struct sh_library_item *it = _it;
    sh_cell_copy(sh_bind(lib, it->key, NULL), &it->value, lib->vm);
  }
}

