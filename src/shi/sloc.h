#ifndef SHI_SLOC
#define SHI_SLOC

struct sh_sloc {
  char source[32];
  char out[64];
  int row;
  int col;
};

struct sh_sloc sh_sloc(const char *source, int row, int col);
const char *sh_sloc_string(struct sh_sloc *sloc);

#endif
