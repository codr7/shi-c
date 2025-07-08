export CC=ccache gcc-15

export CFLAGS=-g -O0 -flto -Wall -Wno-override-init-side-effects -fsanitize=bounds,undefined -Isrc -lm

export LDFLAGS=

CHAPTERS=build/cell.o build/error.o build/evaluate.o build/form.o build/forms/identifier.o build/list.o build/malloc.o build/read.o build/repl.o build/sloc.o build/stack.o build/stream.o build/type.o build/utility.o build/vector.o build/vm.o

all: clean build/all

build/all: src/main.c $(CHAPTERS)
	mkdir -p build/forms
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

build/list.o: src/shi/list.h src/shi/list.c
	$(CC) -c $(CFLAGS) src/shi/list.c -o build/list.o

build/malloc.o: src/shi/malloc.h src/shi/malloc.c
	$(CC) -c $(CFLAGS) src/shi/malloc.c -o build/malloc.o

build/read.o: src/shi/read.h src/shi/read.c
	$(CC) -c $(CFLAGS) src/shi/read.c -o build/read.o

build/repl.o: src/shi/repl.h src/shi/repl.c
	$(CC) -c $(CFLAGS) src/shi/repl.c -o build/repl.o

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
