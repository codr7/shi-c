#ifndef SHI_METHOD
#define SHI_METHOD

#include "shi/utility.h"

struct sh_sloc;
struct sh_stack;
struct sh_vm;

struct sh_argument {
  char name[64];
  struct sh_type *type;
};

struct sh_argument sh_argument(const char *name, struct sh_type *type);

struct sh_method {
  struct sh_library *library;
  char name[64];
  struct sh_argument *arguments;
  int arity;
  int reference_count;
  
  void (*call)(struct sh_method *,
	       size_t *pc,
	       struct sh_stack *,
	       struct sh_sloc *);
};

struct sh_method *sh_method_init(struct sh_method *m,
				 struct sh_library *library,
				 const char *name,
				 int arity,
				 struct sh_argument arguments[]);

struct sh_method *sh_method_acquire(struct sh_method *m);

void sh_method_call(struct sh_method *m,
		    size_t *pc,
		    struct sh_stack *stack,
		    struct sh_sloc *sloc);

void sh_method_release(struct sh_method *m);

struct sh_c_method;

typedef void (*sh_method_body_t)(struct sh_vm *,
				 struct sh_stack *,
				 struct sh_sloc *);

struct sh_c_method {
  struct sh_method method;
  sh_method_body_t body;
};

struct sh_c_method *sh_c_method_init(struct sh_c_method *m,
				     struct sh_library *library,
				     const char *name,
				     int arity,
				     struct sh_argument arguments[],
				     sh_method_body_t body);

#endif
