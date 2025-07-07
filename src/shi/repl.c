#include "shi/malloc.h"
#include "shi/repl.h"
#include "shi/sloc.h"
#include "shi/stack.h"
#include "shi/stream.h"
#include "shi/vm.h"

#include <stdlib.h>
#include <string.h>

void sh_repl(struct sh_vm *vm, FILE *in, FILE *out) {
  struct sh_sloc sloc = sh_sloc("repl", 0, 0);
  struct sh_stack stack;
  sh_stack_init(&stack, vm->malloc);

  struct sh_file_stream in_stream;
  sh_file_stream_init(&in_stream, in);

  struct sh_file_stream out_stream;
  sh_file_stream_init(&out_stream, out);

  struct sh_vector code;
  sh_vector_init(&code, vm->malloc, 1);

  while (!feof(in)) {
    fprintf(out, "  ");
    char *line = sh_gets(&in_stream.stream, vm->malloc);
    if (feof(in)) { break; }

    if (line[0] == '\n') {
      size_t pc = sh_emit_pc(vm);
      printf("code: '%s'\n", code.start);
      sh_vector_clear(&code);
      
      sh_evaluate(vm, &stack, pc, -1);
      sh_stack_dump(&stack, &out_stream.stream);
      fprintf(out, "\n\n");    
    } else {
      sh_vector_grow(&code, code.length + strlen(line));
      strcpy((char *)code.start, line);
      _sh_release(vm->malloc, line);
    }
  }

  sh_stream_deinit(&in_stream.stream);
  sh_stream_deinit(&out_stream.stream);
  sh_vector_deinit(&code);
}
