#ifndef ME_LIB_H
#define ME_LIB_H

#include <pspsdk.h>
#include <pspkernel.h>
#include <malloc.h>
#include <string.h>

#include "me-core-mapper.h"
#include "kernel/kcall.h"

#define ME_HANDLER_BASE                   0xbfc00000
#define ME_EDRAM_BASE                     0x00000000
#define UNCACHED_USER_MASK                0x40000000
#define CACHED_KERNEL_MASK                0x80000000
#define UNCACHED_KERNEL_MASK              0xA0000000

// Lib
#define meLibSync()                       asm volatile("sync")
#define meLibDelayPipeline()              asm volatile("nop; nop; nop; nop; nop; nop; nop; sync;")
#define meLibCallHwMutexTryLock()         (kcall((FCall)(CACHED_KERNEL_MASK | (u32Me)meCoreHwMutexTryLock)))
#define meLibCallHwMutexUnlock()          (kcall((FCall)(CACHED_KERNEL_MASK | (u32Me)meCoreHwMutexUnlock)))
#define meLibEmitSoftwareInterrupt()      (kcall((FCall)(CACHED_KERNEL_MASK | (u32Me)meCoreEmitSoftwareInterrupt)))

#define meLibSetSharedUncachedMem(size) \
  static volatile u32Me _meLibSharedMemory[(size)] __attribute__((aligned(64), section(".uncached"))) = {0}; \
  volatile u32Me* const meLibSharedMemory __attribute__((aligned(64), section(".uncached"))) = \
    (volatile u32Me*)(UNCACHED_USER_MASK | (u32Me)_meLibSharedMemory)

#define meLibMakeUncachedMem(name, size) \
  volatile u32Me name[(size)] __attribute__((aligned(64), section(".uncached"))) = {0};

#define meLibMakeUncachedVar(name, mask) ((volatile u32Me* const)((mask) | (u32Me)name))

inline void meLibHalt() {
  asm volatile(".word 0x70000000");
}

static inline void meLibUnlockHwUserRegisters() {
  const u32Me START = 0xbc000030;
  const u32Me END   = 0xbc000044;
  for(u32Me reg = START; reg <= END; reg+=4) {
    hw(reg) = -1;
  }
  meLibSync();
}

static inline void meLibUnlockMemory() {
  const u32Me START = 0xbc000000;
  const u32Me END   = 0xbc00002c;
  for(u32Me reg = START; reg <= END; reg+=4) {
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

extern char __start__me_section;
extern char __stop__me_section;

extern void meLibOnProcess(void);

#ifdef __cplusplus
extern "C" {
#endif
  extern void meLibSaveContext(void);
  extern void meLibRestoreContext(void);
  extern void meLibOnExternalInterrupt(void);
  extern void meLibOnException(void);
  extern void meLibOnSleep(void);
  extern void meLibOnWake(void);
  int  meLibDefaultInit();
  void meLibExceptionHandlerInit();
  void meLibGetUncached32(volatile u32Me** const mem, const u32Me size);
#ifdef __cplusplus
}
#endif

#endif
