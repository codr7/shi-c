#include "shi/malloc.h"
#include "shi/stack.h"
#include "shi/vm.h"

int main() {
  struct sh_vm vm;
  sh_vm_init(&vm, &sh_malloc_default);

  struct sh_stack s;
  sh_stack_init(&s, &sh_malloc_default);

  //sh_evaluate_s(vm, &s, "say + 1 2");

  sh_evaluate(&vm, &s, 0, -1);
  return 0;
}
