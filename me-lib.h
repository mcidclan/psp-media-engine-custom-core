#ifndef ME_LIB_H
#define ME_LIB_H

#include <pspsdk.h>
#include <pspkernel.h>
#include <psppower.h>
#include <malloc.h>
#include <string.h>
#include "hw-registers.h"
#include "kernel/kcall.h"

#define ERROR_ON_WRITE_PRX  -3
#define ERROR_ON_LOAD_PRX   -4
#define ERROR_ON_KINIT      -5
#define ERROR_ON_ME_IMG     -6

#define meLibSync()                       asm volatile("sync")
#define meLibDelayPipeline()              asm volatile("nop; nop; nop; nop; nop; nop; nop; sync;")
#define meLibCallHwMutexTryLock()         (kcall(meCoreHwMutexTryLock, CACHED_KERNEL_MASK))
#define meLibCallHwMutexUnlock()          (kcall(meCoreHwMutexUnlock, CACHED_KERNEL_MASK))
#define meLibEmitSoftwareInterrupt()      (kcall(meCoreEmitSoftwareInterrupt, CACHED_KERNEL_MASK))

#define meLibSetSharedUncached32(size)    meLibSetSharedUncachedMem(size, u32)

#ifdef __cplusplus
  #define meLibSetSharedUncachedMem(size, type) \
    static volatile type _meLibSharedMemory[(size)] __attribute__((aligned(64), section(".uncached"))) = {(type)0}; \
    volatile type* const meLibSharedMemory __attribute__((aligned(64), section(".uncached"))) = \
      (volatile type*)(UNCACHED_USER_MASK | (uintptr_t)_meLibSharedMemory)

  #define meLibMakeUncachedMem(name, size, type) \
    volatile type name[(size)] __attribute__((aligned(64), section(".uncached"))) = {(type)0};

  #define meLibMakeSharedUncachedMem(name, size, type) \
    static volatile type _meLib_##name[(size)] __attribute__((aligned(64), section(".uncached"))) = {(type)0}; \
    volatile type* const name __attribute__((aligned(64), section(".uncached"))) = \
      (volatile type*)(UNCACHED_USER_MASK | (uintptr_t)_meLib_##name)
#endif


#define meLibMakeMemSegVar(name, mask, type) ((volatile type* const)((mask) | (type)name))

#define defineVar(name) \
  volatile u32 _##name[1] __attribute__((aligned(64), section(".uncached"))) = {0}; \
  volatile u32* name __attribute__((aligned(64), section(".uncached"))) = NULL;

#define buildUncachedVar(name) \
  name = ((volatile u32* const)((UNCACHED_USER_MASK) | (u32)_##name)); \
  name[0] = 0;

// define the non-cached kernel mutex
#define meLibMutex hw(0xbc100048)

#define meLibSuspendCpuIntr(var) \
  asm volatile(                  \
    "mfic  %0, $0 \n"            \
    "mtic  $0, $0 \n"            \
    "sync         \n"            \
    : "=r"(var)                  \
    :                            \
    : "$8"                       \
  )

#define meLibResumeCpuIntr(var)  \
  asm volatile(                  \
    "mtic  %0, $0 \n"            \
    "sync         \n"            \
    :                            \
    : "r"(var)                   \
  )

#ifdef __cplusplus
extern "C" {
#endif

  int  meLibLoadPrx();
  void meLibHalt();
  
  int meLibSendExternalSoftInterrupt();
  u32 meLibGetCpuId();
  // kernel function to unlock the mutex
  int meLibHwMutexUnlock();
  // kernel function that waits and attempts to lock and acquire the mutex
  int meLibHwMutexLock();
  // kernel function to attempt locking and acquiring the mutex
  int meLibHwMutexTryLock();

  // note:
  // it appears that the main CPU can read the mutex and only set bit[0],
  // while the Me can read the mutex and only set bit[1]
  //
  // mutex    unique
  // 11  xor  01 =>   not 10 = 0
  // 11  xor  10 =>   not 01 = 0
  // 10  xor  01 =>   not 11 = 0
  // 10  xor  10 =>   not 00 = 1
  // 01  xor  01 =>   not 00 = 1
  // 01  xor  10 =>   not 11 = 0

  void meLibDcacheWritebackInvalidateAll();
  void meLibDcacheWritebackInvalidateRange(const u32 addr, const u32 size);
  void meLibDcacheInvalidateRange(const u32 addr, const u32 size);
  void meLibDcacheWritebackRange(const u32 addr, const u32 size);

  void meLibIcacheInvalidateAll();
  void meLibIcacheInvalidateRange(const u32 addr, const u32 size);

#ifdef __cplusplus
}
#endif

#endif
