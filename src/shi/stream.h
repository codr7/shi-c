#ifndef SHI_STREAM_H
#define SHI_STREAM_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "vector.h"

char *sh_vsprintf(const char *format, va_list args);

struct sh_stream {  
  size_t (*read)(struct sh_stream *, uint8_t *, size_t);
  size_t (*write)(struct sh_stream *, const uint8_t *, size_t);
  void (*deinit)(struct sh_stream *);
};

size_t sh_read(struct sh_stream *s, uint8_t *data, size_t n);
size_t sh_write(struct sh_stream *s, const uint8_t *data, size_t n);

char sh_getc(struct sh_stream *s);
size_t sh_putc(struct sh_stream *s, char data);
size_t sh_puts(struct sh_stream *s, const char *data);

size_t sh_vprintf(struct sh_stream *s,
		  const char *spec,
		  va_list args);

size_t sh_printf(struct sh_stream *s, const char *spec, ...);

void sh_stream_deinit(struct sh_stream *s);

struct sh_file_stream_opts {
  bool close_file;
};

struct sh_file_stream {
  struct sh_stream stream;
  FILE *file;
  struct sh_file_stream_opts opts;
};

#define sh_file_stream_init(s, f, ...)				\
  _sh_file_stream_init(s, f, (struct sh_file_stream_opts){	\
      .close_file = false,					\
      ##__VA_ARGS__						\
    })

extern struct sh_stream sh_file_stream;

struct sh_file_stream *_sh_file_stream_init(struct sh_file_stream *s,
					    FILE *file,
					    struct sh_file_stream_opts opts);

struct sh_stream *sh_stdout();

struct sh_memory_stream {
  struct sh_stream stream;
  struct sh_vector data;
  size_t rpos;
};

extern struct sh_stream sh_memory_stream;

struct sh_memory_stream *sh_memory_stream_init(struct sh_memory_stream *s,
					       struct sh_malloc *malloc);

const char *sh_memory_stream_string(struct sh_memory_stream *s);

#endif
