# shi-c
a [Simple Hackable Interpreter](https://github.com/codr7/shi) in C

## Implementation
The implementation weighs in at roughly 2.5kloc.

### Values
Values are implemented as tagged unions.

### VM
VM operations are compiled to function pointers followed by optional data blocks.
