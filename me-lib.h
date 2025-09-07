#ifndef ME_LIB_H
#define ME_LIB_H

#include <pspsdk.h>
#include <pspkernel.h>
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
#define meLibDelayPipeline()              asm volatile("nop; nop; nop; nop; nop; nop; nop;")
#define meLibCallHwMutexTryLock()         (kcall((FCall)(CACHED_KERNEL_MASK | (u32)meCoreHwMutexTryLock)))
#define meLibCallHwMutexUnlock()          (kcall((FCall)(CACHED_KERNEL_MASK | (u32)meCoreHwMutexUnlock)))

#define setSharedUncachedMem(name, size) \
  static volatile u32 _##name[(size)] = {0}; \
  static volatile u32* const name = (volatile u32*)(UNCACHED_USER_MASK | (u32)_##name)

inline void meLibHalt() {
  asm volatile(".word 0x70000000");
}

static inline void meLibUnlockHwUserRegisters() {
  const u32 START = 0xbc000030;
  const u32 END   = 0xbc000044;
  for(u32 reg = START; reg <= END; reg+=4) {
    hw(reg) = -1;
  }
  meLibSync();
}

static inline void meLibUnlockMemory() {
  const u32 START = 0xbc000000;
  const u32 END   = 0xbc00002c;
  for(u32 reg = START; reg <= END; reg+=4) {
    hw(reg) = -1;
  }
  meLibSync();
}

static inline void meLibSetMinimalVmeConfig() {
  hw(0xBCC00000) = -1;
  hw(0xBCC00030) = 1;
  hw(0xBCC00040) = 1;
  meLibSync();
}

extern void meLibExec(void);

extern char __start__me_section;
extern char __stop__me_section;

#ifdef __cplusplus
extern "C" {
#endif
int meLibDefaultInit();
void meLibGetUncached32(volatile u32** const mem, const u32 size);
#ifdef __cplusplus
}
#endif

#endif
