#ifndef SHI_METHOD
#define SHI_METHOD

struct sh_sloc;
struct sh_stack;
struct sh_vm;

struct sh_method {
  char *name;
  
  void (*call)(struct sh_method *,
	       struct sh_vm *,
	       struct sh_stack *,
	       struct sh_sloc);
};

#endif
