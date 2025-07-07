#ifndef SHI_REPL
#define SHI_REPL

#include <stdio.h>

struct sh_vm;

void sh_repl(struct sh_vm *vm, FILE *in, FILE *out);

#endif
