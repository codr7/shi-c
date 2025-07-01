#ifndef SHI_VALUE_H
#define SHI_VALUE_H

#include <stdbool.h>

struct sh_cell;
struct sh_stream;

struct sh_type {
  const char *name;
  
  void (*copy)(struct sh_cell *dst, struct sh_cell *src);
  void (*deinit)(struct sh_cell *);
  void (*dump)(const struct sh_cell *, struct sh_stream *out);
  void (*write)(const struct sh_cell *, struct sh_stream *out);
};

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
void sh_cell_dump(struct sh_cell *v, struct sh_stream *out);
void sh_cell_write(struct sh_cell *v, struct sh_stream *out);

extern const struct sh_type SH_BOOL;
extern const struct sh_type SH_FIX;
extern const struct sh_type SH_INT;

#endif
