#ifndef SHI_LIBRARIES_CORE
#define SHI_LIBRARIES_CORE

struct sh_library;
struct sh_vm;

void sh_core_library_init(struct sh_library *lib, struct sh_vm *vm);

struct sh_type *SH_BOOL();
struct sh_type *SH_INT();
struct sh_type *SH_META();
struct sh_type *SH_METHOD();

#endif
