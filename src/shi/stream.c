#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "malloc.h"
#include "stream.h"
#include "utility.h"

char *sh_vsprintf(const char *format, va_list args) {
  va_list tmp_args;
  va_copy(tmp_args, args);
  int len = vsnprintf(NULL, 0, format, tmp_args);
  va_end(tmp_args);

  if (len < 0) {
    sh_throw("Formatting '%s' failed: %d", format, errno);
  }

  len++;
  char *out = malloc(len);
  vsnprintf(out, len, format, args);
  return out;
} 

size_t sh_read(struct sh_stream *s, uint8_t *data, const size_t n) {
  assert(s->read);
  return s->read(s, data, n);
}

size_t sh_write(struct sh_stream *s, const uint8_t *data, const size_t n) {
  assert(s->write);
  return s->write(s, data, n);
}

char sh_getc(struct sh_stream *s) {
  char c = 0;
  return sh_read(s, (uint8_t *)&c, 1) ? c : 0;
}

char *sh_gets(struct sh_stream *s, struct sh_malloc *malloc) {
  struct sh_vector out;
  sh_vector_init(&out, malloc, 1);

  for (;;) {
    char c = sh_getc(s);

    if (c == EOF) {
      break;
    }

    *(char *)sh_vector_push(&out) = c;

    if (c == '\n') {
      break;
    }
  }

 
  *(char *)sh_vector_push(&out) = 0;
  return (char *)out.start;
}

size_t sh_putc(struct sh_stream *s, const char data) {
  const uint8_t d[2] = {data, 0};
  return sh_write(s, d, 1);
}

size_t sh_puts(struct sh_stream *s, const char *data) {
  return sh_write(s, (const uint8_t *)data, strlen(data));
}

size_t sh_vprintf(struct sh_stream *s,
		  const char *spec,
		  va_list args) {
  char *data = sh_vsprintf(spec, args);
  sh_defer(free(data));
  return sh_write(s, (uint8_t *)data, strlen(data));
}

size_t sh_printf(struct sh_stream *s, const char *spec, ...) {
  va_list args;
  va_start(args, spec);
  sh_defer(va_end(args));
  return sh_vprintf(s, spec, args);
}

void sh_stream_deinit(struct sh_stream *s) {
  assert(s->deinit);
  s->deinit(s);
}

size_t file_read(struct sh_stream *s, uint8_t *data, const size_t n) {
  struct sh_file_stream *fs = sh_baseof(s, struct sh_file_stream, stream);
  assert(fs->file);
  return fread(data, n, 1, fs->file);
}

size_t file_write(struct sh_stream *s, const uint8_t *data, const size_t n) {
  struct sh_file_stream *fs = sh_baseof(s, struct sh_file_stream, stream);
  assert(fs->file);
  return fwrite(data, n, 1, fs->file);
}

void file_deinit(struct sh_stream *s) {
  struct sh_file_stream *fs = sh_baseof(s, struct sh_file_stream, stream);

  if (fs->opts.close_file) {
    assert(fs->file);
 
    if (fclose(fs->file) == EOF) {
      sh_throw("Failed closing file");
    }
    
    fs->file = NULL;
  }
}

struct sh_file_stream *_sh_file_stream_init(struct sh_file_stream *s,
					    FILE *file,
					    const struct sh_file_stream_opts opts) {
  s->stream = (struct sh_stream){
    .read   = file_read,
    .write  = file_write,
    .deinit = file_deinit,
  };
  
  s->file = file;
  s->opts = opts;
  return s;
};

struct sh_stream *sh_stdout() {
  static __thread bool init = true;
  static __thread struct sh_file_stream s;

  if (init) {
    sh_file_stream_init(&s, stdout);
    init = false;
  }

  return &s.stream;
}

size_t memory_read(struct sh_stream *s, uint8_t *data, size_t n) {
  struct sh_memory_stream *ms = sh_baseof(s, struct sh_memory_stream, stream);

  if (ms->rpos + n > ms->data.length) {
    n = ms->data.length - ms->rpos;
  }
  
  memcpy(data, ms->data.start + ms->rpos, n);
  ms->rpos += n;
  return n;
}

size_t memory_write(struct sh_stream *s,
		    const uint8_t *data,
		    const size_t n) {
  struct sh_memory_stream *ms = sh_baseof(s, struct sh_memory_stream, stream);
  uint8_t *const dst = sh_vector_insert(&ms->data, ms->data.length, n);
  memcpy(dst, data, n);
  return n;
}

void memory_deinit(struct sh_stream *s) {
  struct sh_memory_stream *ms = sh_baseof(s, struct sh_memory_stream, stream);
  sh_vector_deinit(&ms->data);
}

struct sh_memory_stream *sh_memory_stream_init(struct sh_memory_stream *s,
					       struct sh_malloc *malloc) {
  s->stream = (struct sh_stream){
    .read   = memory_read,
    .write  = memory_write,
    .deinit = memory_deinit,
  };
  
  sh_vector_init(&s->data, malloc, 1);
  s->rpos = 0;
  return s;
}

void sh_memory_stream_reset(struct sh_memory_stream *s) {
  sh_vector_clear(&s->data);
  s->rpos = 0;
}

char *sh_memory_stream_string(struct sh_memory_stream *s) {
  if (!s->data.length || (*(s->data.end-1))) {
    *(uint8_t *)sh_vector_push(&s->data) = 0;
  }

  return (char *)s->data.start;
}
