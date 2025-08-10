#ifndef SHI_TYPE
#define SHI_TYPE

#include <stdbool.h>
#include "shi/set.h"

struct sh_cell;
struct sh_list;
struct sh_sloc;
struct sh_stream;
struct sh_vm;

struct sh_type {
  char *name;
  struct sh_set parents;
  int reference_count;
  
  bool (*as_bool)(struct sh_cell *);
  void (*copy)(struct sh_cell *, struct sh_cell *, struct sh_vm *vm);
  void (*deinit)(struct sh_cell *);
  void (*dump)(const struct sh_cell *, struct sh_stream *);
  void (*emit)(struct sh_cell *, struct sh_vm *, struct sh_sloc, struct sh_list *); 
  bool (*eq)(const struct sh_cell *, const struct sh_cell *);
};

struct sh_type *sh_type_init(struct sh_type *t, const char *name);

struct sh_type *sh_type_acquire(struct sh_type *t);
void sh_type_release(struct sh_type *t);

#endif
