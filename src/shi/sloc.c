#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "shi/sloc.h"

struct sh_sloc sh_sloc(const char *source, const int line, const int column) {
  struct sh_sloc s = {.source = {0}, .line = line, .column = column};
  assert(strlen(source) < sizeof(s.source));
  strcpy(s.source, source);
  return s;
}

void sh_sloc_step(struct sh_sloc *sloc, char c) {
  if (c == '\n') {
    sloc->line++;
    sloc->column = 1;
  } else {
    sloc->column++;
  }
}

const char *sh_sloc_string(struct sh_sloc *sloc) {
  snprintf(sloc->out, sizeof(sloc->out), "'%s'; line %d, column %d",
	   sloc->source, sloc->line, sloc->column);
  
  return sloc->out;
}
