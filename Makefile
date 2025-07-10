export CC=ccache gcc-15

export CFLAGS=-g -O0 -flto -Wall -Wno-override-init-side-effects -fsanitize=bounds,undefined -Isrc -lm

export LDFLAGS=

CHAPTERS=build/cell.o build/error.o build/evaluate.o build/form.o build/forms/identifier.o build/forms/literal.o build/library.o build/libraries/core.o build/libraries/core/bool.o build/libraries/core/int.o build/libraries/core/method.o build/list.o build/malloc.o build/read.o build/repl.o build/set.o build/sloc.o build/stack.o build/stream.o build/type.o build/utility.o build/vector.o build/vm.o

all: clean build/all

build/all: src/main.c $(CHAPTERS)
	$(CC) $(CFLAGS) src/main.c $(CHAPTERS) -o build/shi
	valgrind build/shi

build/cell.o: src/shi/cell.h src/shi/cell.c
	$(CC) -c $(CFLAGS) src/shi/cell.c -o build/cell.o

build/error.o: src/shi/error.h src/shi/error.c
	$(CC) -c $(CFLAGS) src/shi/error.c -o build/error.o

build/evaluate.o: src/shi/evaluate.h src/shi/evaluate.c
	$(CC) -c $(CFLAGS) src/shi/evaluate.c -o build/evaluate.o

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

build/libraries/core/bool.o: src/shi/libraries/core/bool.h src/shi/libraries/core/bool.c
	$(CC) -c $(CFLAGS) src/shi/libraries/core/bool.c -o build/libraries/core/bool.o

build/libraries/core/int.o: src/shi/libraries/core/int.h src/shi/libraries/core/int.c
	$(CC) -c $(CFLAGS) src/shi/libraries/core/int.c -o build/libraries/core/int.o

build/libraries/core/method.o: src/shi/libraries/core/method.h src/shi/libraries/core/method.c
	$(CC) -c $(CFLAGS) src/shi/libraries/core/method.c -o build/libraries/core/method.o

build/list.o: src/shi/list.h src/shi/list.c
	$(CC) -c $(CFLAGS) src/shi/list.c -o build/list.o

build/malloc.o: src/shi/malloc.h src/shi/malloc.c
	$(CC) -c $(CFLAGS) src/shi/malloc.c -o build/malloc.o

build/read.o: src/shi/read.h src/shi/read.c
	$(CC) -c $(CFLAGS) src/shi/read.c -o build/read.o

build/repl.o: src/shi/repl.h src/shi/repl.c
	$(CC) -c $(CFLAGS) src/shi/repl.c -o build/repl.o

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
