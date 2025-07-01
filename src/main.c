#include "shi/malloc.h"
#include "shi/stack.h"
#include "shi/vm.h"

int main() {
  struct sh_vm vm;
  sh_vm_init(&vm, &sh_malloc_default);

  sh_stack_t s;
  sh_stack_init(&s, &sh_malloc_default);
  
  return 0;
}
