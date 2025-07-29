#include "shi/malloc.h"
#include "shi/stack.h"
#include "shi/vm.h"

int main(int argc, const char **argv) {
  struct sh_vm vm;
  sh_vm_init(&vm, &sh_malloc_default);
  sh_defer(sh_vm_deinit(&vm));

  if (argc == 1) {
    sh_shell(&vm, stdin, stdout);
  } else {
    size_t start_pc = sh_emit_pc(&vm);
    struct sh_stack stack;
    sh_defer(sh_stack_deinit(&stack));
    sh_stack_init(&stack, vm.malloc);
    
    for (const char **p = argv+1; *p; p++) {
      sh_load(&vm, *p);
    }

    sh_evaluate(&vm, &stack, start_pc, -1);
  }
  
  return 0;
}
