#include "shi/form.h"
#include "shi/list.h"
#include "shi/malloc.h"
#include "shi/read.h"
#include "shi/sloc.h"
#include "shi/stack.h"
#include "shi/stream.h"
#include "shi/vm.h"

#include <stdlib.h>
#include <string.h>

void sh_shell(struct sh_vm *vm, FILE *in, FILE *out) {
  struct sh_sloc sloc = sh_sloc("repl", 0, 0);
  struct sh_stack stack;
  sh_stack_init(&stack, vm->malloc);
  sh_defer(sh_stack_deinit(&stack));
  
  struct sh_file_stream in_stream;
  sh_file_stream_init(&in_stream, in);
  sh_defer(sh_stream_deinit(&in_stream.stream));

  struct sh_file_stream out_stream;
  sh_file_stream_init(&out_stream, out);
  sh_defer(sh_stream_deinit(&out_stream.stream));

  struct sh_memory_stream code;
  sh_memory_stream_init(&code, vm->malloc);
  sh_defer(sh_stream_deinit(&code.stream));
  int line_count = 0;
  
  while (!feof(in)) {
    fprintf(out, "% 2d ", sloc.line + line_count);
    char *line = sh_gets(&in_stream.stream, vm->malloc);
    sh_defer(sh_release(vm->malloc, line));
    if (feof(in)) { break; }

    if (strcmp(":q\n", line) == 0 && !line_count) {
      break;
    } else if (line[0] == '\n') {
      struct sh_list forms;
      sh_list_init(&forms);
      const char *cs = sh_memory_stream_string(&code);
      sh_read_forms(vm, &cs, &forms, &sloc);
      sh_memory_stream_reset(&code);
      line_count = 0;
      //sh_forms_dump(&forms, &out_stream.stream);
      const size_t pc = sh_emit_pc(vm);      
      sh_forms_emit(&forms, vm);
      sh_forms_release(&forms, vm);
      sh_evaluate(vm, &stack, pc, -1);
      sh_stack_dump(&stack, &out_stream.stream);
      fprintf(out, "\n\n");
    } else {
      sh_puts(&code.stream, line);
      sh_release(vm->malloc, line);
      line_count++;
    }
  }
}
