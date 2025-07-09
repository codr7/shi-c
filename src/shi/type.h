#ifndef SHI_TYPE
#define SHI_TYPE

#include <stdbool.h>

struct sh_cell;
struct sh_stream;

struct sh_type {
  const char *name;
  
  void (*copy)(struct sh_cell *, struct sh_cell *);
  void (*deinit)(struct sh_cell *);
  void (*dump)(const struct sh_cell *, struct sh_stream *);
  void (*emit)(struct sh_cell *, struct sh_vm *, struct sh_sloc);  
  void (*write)(const struct sh_cell *, struct sh_stream *);
};

extern const struct sh_type SH_BOOL;
extern const struct sh_type SH_INT;
extern const struct sh_type SH_METHOD;

#endif
