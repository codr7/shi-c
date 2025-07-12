#ifndef SHI_METHOD
#define SHI_METHOD

struct sh_sloc;
struct sh_stack;
struct sh_vm;

struct sh_argument {
  char name[64];
  struct sh_type *type;
};

struct sh_method {
  struct sh_library *library;
  char name[64];
  struct sh_argument *arguments;
  int arity;
  
  void (*call)(struct sh_method *,
	       struct sh_vm *,
	       struct sh_stack *,
	       const struct sh_sloc *);
};

struct sh_method *sh_method_init(struct sh_method *m,
				 struct sh_library *library,
				 const char *name,
				 int arity,
				 struct sh_argument arguments[]);

void sh_method_free(struct sh_method *m);

typedef void (*sh_method_body_t)(struct sh_vm *,
				 struct sh_stack *stack,
				 const struct sh_sloc *sloc);

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
