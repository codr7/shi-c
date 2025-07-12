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
  char name[64];
  
  void (*call)(struct sh_method *,
	       struct sh_vm *,
	       struct sh_stack *,
	       const struct sh_sloc *);
};

struct sh_method *sh_method_init(struct sh_method *m,
				 const char *name,
				 int arity,
				 struct sh_argument args[]);

typedef void (*sh_method_body_t)(struct sh_vm *,
				 struct sh_stack *stack,
				 const struct sh_sloc *sloc);

struct sh_c_method {
  struct sh_method method;
  sh_method_body_t body;
};

struct sh_c_method *sh_c_method_init(struct sh_c_method *m,
				     const char *name,
				     int arity,
				     struct sh_argument args[],
				     sh_method_body_t body);

#endif
