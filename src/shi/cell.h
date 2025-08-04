#ifndef SHI_CELL
#define SHI_CELL

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct sh_list;
struct sh_sloc;
struct sh_stream;
struct sh_type;
struct sh_vm;

struct sh_cell {
  struct sh_type *type;
  
  union {
    bool as_bool;
    int64_t as_int;
    void *as_other;
    size_t as_register;
  };
};

struct sh_cell *sh_cell_init(struct sh_cell *v, struct sh_type *t);
void sh_cell_deinit(struct sh_cell *v);

bool sh_as_bool(struct sh_cell *v);

struct sh_cell *sh_cell_copy(struct sh_cell *dst,
			     struct sh_cell *src,
			     struct sh_vm *vm);

void sh_cell_dump(const struct sh_cell *v, struct sh_stream *out);

void sh_cell_emit(struct sh_cell *v,
		  struct sh_vm *vm,
		  struct sh_sloc sloc,
		  struct sh_list *args);

bool sh_eq(const struct sh_cell *x, const struct sh_cell *y);

bool sh_isa(const struct sh_cell *v, const struct sh_type *t);

#endif
