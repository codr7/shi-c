#ifndef SHI_SLOC
#define SHI_SLOC

struct sh_sloc {
  char source[32];
  char out[64];
  int line;
  int column;
};

struct sh_sloc sh_sloc(const char *source, int line, int column);
void sh_sloc_step(struct sh_sloc *sloc, char c);
const char *sh_sloc_string(struct sh_sloc *sloc);

#endif
