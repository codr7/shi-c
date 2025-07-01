export CC=ccache gcc-15
export CFLAGS=-g -O0 -flto -Wall -Wno-override-init-side-effects -fsanitize=bounds,undefined -Isrc -lm
export LDFLAGS=

CHAPTERS=build/cell.o build/error.o build/macro.o build/malloc.o build/sloc.o build/stack.o build/stream.o build/vector.o build/vm.o

all: clean build/all

build/all: src/main.c $(CHAPTERS) 
	$(CC) $(CFLAGS) src/main.c $(CHAPTERS) -o build/shi
	valgrind build/shi

build/cell.o: src/shi/cell.h src/shi/cell.c
	$(CC) -c $(CFLAGS) src/shi/cell.c -o build/cell.o

build/error.o: src/shi/error.h src/shi/error.c
	$(CC) -c $(CFLAGS) src/shi/error.c -o build/error.o

build/macro.o: src/shi/macro.h src/shi/macro.c
	$(CC) -c $(CFLAGS) src/shi/macro.c -o build/macro.o

build/malloc.o: src/shi/malloc.h src/shi/malloc.c
	$(CC) -c $(CFLAGS) src/shi/malloc.c -o build/malloc.o

build/sloc.o: src/shi/sloc.h src/shi/sloc.c
	$(CC) -c $(CFLAGS) src/shi/sloc.c -o build/sloc.o

build/stack.o: src/shi/stack.h src/shi/stack.c
	$(CC) -c $(CFLAGS) src/shi/stack.c -o build/stack.o

build/stream.o: src/shi/stream.h src/shi/stream.c
	$(CC) -c $(CFLAGS) src/shi/stream.c -o build/stream.o

build/vector.o: src/shi/vector.h src/shi/vector.c
	$(CC) -c $(CFLAGS) src/shi/vector.c -o build/vector.o

build/vm.o: src/shi/vm.h src/shi/vm.c
	$(CC) -c $(CFLAGS) src/shi/vm.c -o build/vm.o

clean:
	rm -f build/*
