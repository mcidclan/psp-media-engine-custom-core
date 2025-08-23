#ifndef ME_LIB_H
#define ME_LIB_H

#include <malloc.h>
#include <string.h>

#include "me-core-mapper.h"
#include "kcall.h"

#define ME_HANDLER_BASE                   0xbfc00000
#define ME_EDRAM_BASE                     0x00000000
#define UNCACHED_USER_MASK                0x40000000
#define CACHED_KERNEL_MASK                0x80000000
#define UNCACHED_KERNEL_MASK              0xA0000000

// Lib
#define meLibSync()                       asm volatile("sync")

inline void meLibHalt() {
  asm volatile(".word 0x70000000");
}

inline void meLibGetUncached32(volatile u32** const mem, const u32 size) {
  static void* _base = nullptr;
  if (!_base) {
    _base = memalign(16, size*4);
    memset(_base, 0, size);
    *mem = (u32*)(UNCACHED_USER_MASK | (u32)_base);
    sceKernelDcacheWritebackInvalidateAll();
    return;
  } else if (!size) {
    free(_base);
  }
  *mem = nullptr;
  return;
}

extern void meLibExec(void);

extern char __start__me_section;
extern char __stop__me_section;

__attribute__((section("_me_section")))
void meLibHandler() {
  asm volatile(
    "li          $t0, 0x30000000\n"
    "mtc0        $t0, $12\n"
    "sync\n"
    : : : "t0"
  );
  meLibExec();
}

inline int meLibInit() {
  const int tableId = meCoreGetTableIdFromWitnessWord();
  if (tableId < 2) {
    return -1;
  }
  meCoreSelectSystemTable(tableId);
  #define me_section_size (&__stop__me_section - &__start__me_section)
  memcpy((void *)ME_HANDLER_BASE, (void*)&__start__me_section, me_section_size);
  sceKernelDcacheWritebackInvalidateAll();
  HW_SYS_RESET_ENABLE = 0x04;
  HW_SYS_RESET_ENABLE = 0x00;
  meLibSync();
  return tableId;
}

inline int meLibDefaultInit() {
  if (pspSdkLoadStartModule("./kcall.prx", PSP_MEMORY_PARTITION_KERNEL) < 0){
    sceKernelExitGame();
    return -3;
  }
  return kcall(meLibInit);
}
  

#endif
