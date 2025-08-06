#include "shi/malloc.h"
#include "shi/stack.h"
#include "shi/vm.h"

int main(int argc, const char **argv) {
  struct sh_bump_alloc m;
  sh_bump_alloc_init(&m, &sh_malloc_default, 100000);
  sh_defer(sh_bump_alloc_deinit(&m));
	   
  struct sh_vm vm;
  sh_vm_init(&vm, &m.malloc);
  sh_defer(sh_vm_deinit(&vm));
  
  if (argc == 1) {
    sh_shell(&vm, stdin, stdout);
  } else {
    const size_t start_pc = sh_emit_pc(&vm);
    struct sh_stack stack;
    sh_stack_init(&stack, vm.malloc);
    sh_defer(sh_stack_deinit(&stack));
    
    for (const char **p = argv+1; *p; p++) {
      sh_load(&vm, *p);
    }

    sh_evaluate(&vm, &stack, start_pc, -1);
  }
  
  return 0;
}
