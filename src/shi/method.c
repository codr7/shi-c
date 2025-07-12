#include <string.h>

#include "shi/method.h"

struct sh_method *sh_method_init(struct sh_method *m,
				 const char *name,
				 int arity,
				 struct sh_argument args[]) {
  strcpy(m->name, name);
  m->call = NULL;
  return m;
}

struct sh_c_method *sh_c_method_init(struct sh_c_method *m,
				     const char *name,
				     int arity,
				     struct sh_argument args[],
				     sh_method_body_t body) {
  
  sh_method_init(&m->method, name, arity, args);
  m->body = body;
  return m;  
}
