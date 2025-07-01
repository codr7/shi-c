#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "shi/sloc.h"

struct sh_sloc sh_sloc(const char *source, const int row, const int col) {
  struct sh_sloc s = {.source = {0}, .row = row, .col = col};
  assert(strlen(source) < sizeof(s.source));
  strcpy(s.source, source);
  return s;
}

const char *sh_sloc_string(struct sh_sloc *sloc) {
  snprintf(sloc->out, sizeof(sloc->out), "'%s'; row %d, column %d",
	  sloc->source, sloc->row, sloc->col);
  
  return sloc->out;
}
