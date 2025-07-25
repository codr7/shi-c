export CC=ccache gcc-15

export CFLAGS=-g -O0 -flto -Wall -Wno-override-init-side-effects -fsanitize=bounds,undefined -Isrc -lm

export LDFLAGS=

CHAPTERS=build/call.o build/cell.o build/error.o build/form.o build/forms/identifier.o build/forms/literal.o build/library.o build/libraries/core.o build/libraries/core/any.o build/libraries/core/bool.o build/libraries/core/int.o build/libraries/core/macro.o build/libraries/core/meta.o build/libraries/core/method.o build/list.o build/macro.o build/malloc.o build/method.o build/operations/branch.o build/operations/call_method.o build/operations/check_value.o build/operations/goto.o build/operations/push_value.o build/operations/return.o build/read.o build/set.o build/shell.o build/sloc.o build/stack.o build/stream.o build/type.o build/utility.o build/vector.o build/vm.o

all: clean build/all

build/all: src/main.c $(CHAPTERS)
	$(CC) $(CFLAGS) src/main.c $(CHAPTERS) -o build/shi
	valgrind build/shi

build/call.o: src/shi/call.h src/shi/call.c
	$(CC) -c $(CFLAGS) src/shi/call.c -o build/call.o

build/cell.o: src/shi/cell.h src/shi/cell.c
	$(CC) -c $(CFLAGS) src/shi/cell.c -o build/cell.o

build/error.o: src/shi/error.h src/shi/error.c
	$(CC) -c $(CFLAGS) src/shi/error.c -o build/error.o

build/form.o: src/shi/form.h src/shi/form.c
	$(CC) -c $(CFLAGS) src/shi/form.c -o build/form.o

build/forms/identifier.o: src/shi/forms/identifier.h src/shi/forms/identifier.c
	$(CC) -c $(CFLAGS) src/shi/forms/identifier.c -o build/forms/identifier.o

build/forms/literal.o: src/shi/forms/literal.h src/shi/forms/literal.c
	$(CC) -c $(CFLAGS) src/shi/forms/literal.c -o build/forms/literal.o

build/library.o: src/shi/library.h src/shi/library.c
	$(CC) -c $(CFLAGS) src/shi/library.c -o build/library.o

build/libraries/core.o: src/shi/libraries/core.h src/shi/libraries/core.c
	$(CC) -c $(CFLAGS) src/shi/libraries/core.c -o build/libraries/core.o

build/libraries/core/any.o: src/shi/libraries/core/any.c
	$(CC) -c $(CFLAGS) src/shi/libraries/core/any.c -o build/libraries/core/any.o

build/libraries/core/bool.o: src/shi/libraries/core/bool.c
	$(CC) -c $(CFLAGS) src/shi/libraries/core/bool.c -o build/libraries/core/bool.o

build/libraries/core/int.o: src/shi/libraries/core/int.c
	$(CC) -c $(CFLAGS) src/shi/libraries/core/int.c -o build/libraries/core/int.o

build/libraries/core/macro.o: src/shi/libraries/core/macro.c
	$(CC) -c $(CFLAGS) src/shi/libraries/core/macro.c -o build/libraries/core/macro.o

build/libraries/core/meta.o: src/shi/libraries/core/meta.c
	$(CC) -c $(CFLAGS) src/shi/libraries/core/meta.c -o build/libraries/core/meta.o

build/libraries/core/method.o: src/shi/libraries/core/method.c
	$(CC) -c $(CFLAGS) src/shi/libraries/core/method.c -o build/libraries/core/method.o

build/list.o: src/shi/list.h src/shi/list.c
	$(CC) -c $(CFLAGS) src/shi/list.c -o build/list.o

build/macro.o: src/shi/macro.h src/shi/macro.c
	$(CC) -c $(CFLAGS) src/shi/macro.c -o build/macro.o

build/malloc.o: src/shi/malloc.h src/shi/malloc.c
	$(CC) -c $(CFLAGS) src/shi/malloc.c -o build/malloc.o

build/method.o: src/shi/method.h src/shi/method.c
	$(CC) -c $(CFLAGS) src/shi/method.c -o build/method.o

build/operations/branch.o: src/shi/operations/branch.h src/shi/operations/branch.c
	$(CC) -c $(CFLAGS) src/shi/operations/branch.c -o build/operations/branch.o

build/operations/call_method.o: src/shi/operations/call_method.h src/shi/operations/call_method.c
	$(CC) -c $(CFLAGS) src/shi/operations/call_method.c -o build/operations/call_method.o

build/operations/check_value.o: src/shi/operations/check_value.h src/shi/operations/check_value.c
	$(CC) -c $(CFLAGS) src/shi/operations/check_value.c -o build/operations/check_value.o

build/operations/goto.o: src/shi/operations/goto.h src/shi/operations/goto.c
	$(CC) -c $(CFLAGS) src/shi/operations/goto.c -o build/operations/goto.o

build/operations/push_value.o: src/shi/operations/push_value.h src/shi/operations/push_value.c
	$(CC) -c $(CFLAGS) src/shi/operations/push_value.c -o build/operations/push_value.o

build/operations/return.o: src/shi/operations/return.h src/shi/operations/return.c
	$(CC) -c $(CFLAGS) src/shi/operations/return.c -o build/operations/return.o

build/read.o: src/shi/read.h src/shi/read.c
	$(CC) -c $(CFLAGS) src/shi/read.c -o build/read.o

build/shell.o: src/shi/shell.c
	$(CC) -c $(CFLAGS) src/shi/shell.c -o build/shell.o

build/set.o: src/shi/set.h src/shi/set.c
	$(CC) -c $(CFLAGS) src/shi/set.c -o build/set.o

build/sloc.o: src/shi/sloc.h src/shi/sloc.c
	$(CC) -c $(CFLAGS) src/shi/sloc.c -o build/sloc.o

build/stack.o: src/shi/stack.h src/shi/stack.c
	$(CC) -c $(CFLAGS) src/shi/stack.c -o build/stack.o

build/stream.o: src/shi/stream.h src/shi/stream.c
	$(CC) -c $(CFLAGS) src/shi/stream.c -o build/stream.o

build/type.o: src/shi/type.h src/shi/type.c
	$(CC) -c $(CFLAGS) src/shi/type.c -o build/type.o

build/utility.o: src/shi/utility.h src/shi/utility.c
	$(CC) -c $(CFLAGS) src/shi/utility.c -o build/utility.o

build/vector.o: src/shi/vector.h src/shi/vector.c
	$(CC) -c $(CFLAGS) src/shi/vector.c -o build/vector.o

build/vm.o: src/shi/vm.h src/shi/vm.c
	$(CC) -c $(CFLAGS) src/shi/vm.c -o build/vm.o

clean:
	rm -rf build/*
	mkdir build/forms
	mkdir build/libraries
	mkdir build/libraries/core
	mkdir build/operations
