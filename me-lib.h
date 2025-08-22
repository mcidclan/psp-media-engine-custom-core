#ifndef ME_LIB_H
#define ME_LIB_H

#include <malloc.h>
#include <string.h>
#include "me-core-mapper.h"

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

#endif
