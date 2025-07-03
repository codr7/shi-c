#ifndef SHI_ERROR
#define SHI_ERROR

#include <stdbool.h>
#include <setjmp.h>

#include "utility.h"

#define SH_NO_MEMORY "NO_MEMORY"
#define SH_INVALID_SIZE "INVALID_SIZE"

#define sh_throw(m, ...) do {					\
    struct sh_error *_e =					\
      sh_error_new("Error in '%s', line %d:\n" m "\n",		\
		   __FILE__, __LINE__, ##__VA_ARGS__);		\
    _sh_throw(_e);						\
  } while(0)

#define _sh_catch(_e, _f, h)						\
    jmp_buf _e;								\
    bool _f = true;							\
    if (setjmp(_e)) {							\
      h(sh_error);							\
      sh_error_free(sh_error);						\
    } else for (sh_catch_push(_e); _f; _f = false, sh_catch_pop())	\

#define sh_catch(h)				\
  _sh_catch(sh_unique(env), sh_unique(flag), h)

void sh_catch_push(jmp_buf h);
void sh_catch_pop();
void sh_errors_deinit();

struct sh_error {
  char *message;
};


extern __thread struct sh_error *sh_error;
struct sh_error *sh_error_new(const char *message, ...);
void _sh_throw(struct sh_error *e);
void sh_error_free(struct sh_error *e);

bool sh_streq(const char *l, const char *r);

#endif
