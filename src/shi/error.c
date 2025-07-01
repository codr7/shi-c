#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "malloc.h"
#include "vector.h"

__thread struct sh_error *sh_error = NULL;

static struct sh_vector *handlers() {
  static bool init = true;
  static __thread struct sh_vector handlers;

  if (init) {
    sh_vector_init(&handlers, &sh_malloc_default, sizeof(jmp_buf));
    init = false;
  }
  
  return &handlers;
}

void sh_catch_push(jmp_buf h) {
  memcpy((jmp_buf *)sh_vector_push(handlers()), h, sizeof(jmp_buf));
}

void sh_catch_pop() {
  sh_vector_pop(handlers());
}

void sh_errors_deinit() {
  sh_vector_deinit(handlers());
}

void _sh_throw(struct sh_error *e) {
  struct sh_vector *hs = handlers();

  if (!hs->length) {
    fputs(e->message, stderr);
    sh_error_free(e);
    abort();
  }
  
  jmp_buf t;
  memcpy(t, *(jmp_buf *)sh_vector_pop(hs), sizeof(jmp_buf));
  sh_error = e;
  longjmp(t, 1);
}

struct sh_error *sh_error_new(const char *message, ...) {
  va_list args;
  va_start(args, message);
  
  va_list tmp_args;
  va_copy(tmp_args, args);
  int len = vsnprintf(NULL, 0, message, tmp_args);
  va_end(tmp_args);

  if (len < 0) {
    vfprintf(stderr, message, args);
    abort();
  }
  
  len++;
  struct sh_error *e = malloc(sizeof(struct sh_error));
  e->message = malloc(len);
  vsnprintf(e->message, len, message, args);
  va_end(args);
  return e;
}

void sh_error_free(struct sh_error *e) {
  free(e->message);
  free(e);
}

bool sh_streq(const char *l, const char *r) {
  for (; *l && *l == *r; l++, r++);
  return *l == *r;
}
