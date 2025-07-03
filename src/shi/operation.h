#ifndef SHI_OPERATION
#define SHI_OPERATION

#include <stddef.h>
#include <stdint.h>

struct sh_stack;
struct sh_vm;

typedef uint8_t *(*sh_evaluate_t)(struct sh_vm *,
				  struct sh_stack *stack,
				  uint8_t *);

struct sh_operation {
  const char *name;

  size_t align;
  size_t size;

  sh_evaluate_t evaluate;
  void (*deinit)(uint8_t *);
};

#endif
