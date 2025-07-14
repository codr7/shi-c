#ifndef SHI_TYPE
#define SHI_TYPE

#include <stdbool.h>

#define SH_TYPE_DEFAULTS				\
  .copy = sh_type_default_copy,				\
    .emit = sh_type_default_emit

struct sh_cell;
struct sh_list;
struct sh_sloc;
struct sh_stream;
struct sh_vm;

struct sh_type {
  const char *name;
  
  void (*copy)(struct sh_cell *, struct sh_cell *, struct sh_vm *vm);
  void (*deinit)(struct sh_cell *);
  void (*dump)(const struct sh_cell *, struct sh_stream *);
  void (*emit)(struct sh_cell *, struct sh_vm *, struct sh_sloc, struct sh_list *); 
  bool (*eq)(const struct sh_cell *, const struct sh_cell *);
};

void sh_type_default_copy(struct sh_cell *dst, struct sh_cell *src,
			  struct sh_vm *vm);

void sh_type_default_emit(struct sh_cell *v,
			  struct sh_vm *vm,
			  struct sh_sloc sloc,
			  struct sh_list *args);

#endif
