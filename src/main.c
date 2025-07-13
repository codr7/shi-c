#include "shi/malloc.h"
#include "shi/stack.h"
#include "shi/vm.h"

int main() {
  struct sh_vm vm;
  sh_vm_init(&vm, &sh_malloc_default);
  sh_shell(&vm, stdin, stdout);
  return 0;
}
