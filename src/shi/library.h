#ifndef SHI_LIBRARY
#define SHI_LIBRARY

#include "shi/macro.h"
#include "shi/method.h"
#include "shi/set.h"

struct sh_cell;
struct sh_type;

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

struct sh_cell *sh_bind(struct sh_library *lib,
			const char *key,
			struct sh_type *type);

struct sh_c_macro *sh_bind_macro(struct sh_library *lib,
				 const char *name,
				 int arity,
				 sh_macro_argument_t arguments[],
				 sh_macro_body_t body);

struct sh_c_method *sh_bind_method(struct sh_library *lib,
				   const char *name,
				   int arity,
				   struct sh_argument arguments[],
				   sh_method_body_t body);

void sh_bind_type(struct sh_library *lib, struct sh_type *type);

struct sh_cell *sh_find(struct sh_library *lib, const char *key);
void sh_import(struct sh_library *lib, struct sh_library *source);

#endif
