# shi-c
a [Simple Hackable Interpreter](https://github.com/codr7/shi) in C

## Running
The following shell spell should take you from zero to REPL, given a reasonably modern C compiler and `ccache`:

```
git clone https://github.com/codr7/shi-c
cd shi-c
make
```

## Implementation
The code weighs in at roughly 2.5kloc.

Much of the basic infrastructure comes from [Hacktical C](https://github.com/codr7/hacktical-c).

### Values
Values are implemented as tagged unions.

### VM
VM operations are compiled to function pointers, optionally followed by a chunk of arguments.
