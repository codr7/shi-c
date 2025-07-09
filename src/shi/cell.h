#ifndef SHI_CELL
#define SHI_CELL

#include <stdbool.h>

struct sh_sloc;
struct sh_stream;
struct sh_type;
struct sh_vm;

struct sh_cell {
  const struct sh_type *type;
  
  union {
    bool as_bool;
    int as_int;
    void *as_pointer;
  };
};

struct sh_cell *sh_cell_init(struct sh_cell *v, const struct sh_type *t);
void sh_cell_deinit(struct sh_cell *v);
struct sh_cell *sh_cell_copy(struct sh_cell *dst, struct sh_cell *src);
void sh_cell_dump(const struct sh_cell *v, struct sh_stream *out);
void sh_cell_emit(struct sh_cell *v, struct sh_vm *vm, struct sh_sloc sloc);
void sh_cell_write(const struct sh_cell *v, struct sh_stream *out);

extern const struct sh_type SH_BOOL;
extern const struct sh_type SH_FIX;
extern const struct sh_type SH_INT;

#endif
