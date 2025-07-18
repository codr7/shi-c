#ifndef SHI_MACRO
#define SHI_MACRO

#include "shi/list.h"
#include "shi/utility.h"

#define sh_macro_arguments(...)			\
  (sh_macro_argument_t[]) { __VA_ARGS__ }

struct sh_sloc;
struct sh_stack;
struct sh_vm;

typedef char sh_macro_argument_t[64];

struct sh_macro {
  struct sh_library *library;
  char name[64];
  sh_macro_argument_t *arguments;
  int arity;
  int reference_count;
  
  void (*emit)(struct sh_macro *,
	       struct sh_sloc *,
	       struct sh_list *);
};

struct sh_macro *sh_macro_init(struct sh_macro *m,
			       struct sh_library *library,
			       const char *name,
			       int arity,
			       sh_macro_argument_t arguments[]);

struct sh_macro *sh_macro_acquire(struct sh_macro *m);

void sh_macro_emit(struct sh_macro *m,
		   struct sh_sloc *sloc,
		   struct sh_list *arguments);

void sh_macro_release(struct sh_macro *m);

struct sh_c_macro;

typedef void (*sh_macro_body_t)(struct sh_vm *,
				struct sh_sloc *,
				struct sh_list *);

struct sh_c_macro {
  struct sh_macro macro;
  sh_macro_body_t body;
};

struct sh_c_macro *sh_c_macro_init(struct sh_c_macro *m,
				   struct sh_library *library,
				   const char *name,
				   int arity,
				   sh_macro_argument_t arguments[],
				   sh_macro_body_t body);

#endif
