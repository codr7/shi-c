#include <stdlib.h>
#include <stdio.h>

#include "error.h"
#include "malloc.h"
#include "utility.h"

static void *default_acquire(struct sh_malloc *m, size_t size) {
  return malloc(size);
}

static void default_release(struct sh_malloc *m, void *p) {
  free(p);
}

struct sh_malloc sh_malloc_default = {.acquire = default_acquire,
				      .release = default_release};

__thread struct sh_malloc *sh_mallocp = NULL;

struct sh_malloc *sh_malloc() {
  return sh_mallocp ? sh_mallocp : &sh_malloc_default;
}

/* Bump */

static void *bump_acquire(struct sh_malloc *a, size_t size) {
  if (size <= 0) {
    sh_throw(SH_INVALID_SIZE);
  } 

  struct sh_bump_alloc *ba = sh_baseof(a, struct sh_bump_alloc, malloc);
  
  if (ba->size - ba->offset < size) {
    sh_throw(SH_NO_MEMORY);
  } 

  uint8_t *p = ba->memory + ba->offset;
  uint8_t *pa = sh_align(p, size);
  ba->offset = ba->offset + pa - p + size;
  return pa;
}

static void bump_release(struct sh_malloc *a, void *p) {
  //Do nothing
}

void sh_bump_alloc_init(struct sh_bump_alloc *a,
			struct sh_malloc *source,
			size_t size) {
  a->malloc.acquire = bump_acquire;
  a->malloc.release = bump_release;
  a->source = source;
  a->size = size;
  a->offset = 0;
  a->memory = _sh_acquire(source, size);
}

void sh_bump_alloc_deinit(struct sh_bump_alloc *a) {
  _sh_release(a->source, a->memory);
}
