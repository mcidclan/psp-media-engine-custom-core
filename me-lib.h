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
#define meLibCallHwMutexTryLock()         (kcall(meCoreHwMutexTryLock, 1))
#define meLibCallHwMutexUnlock()          (kcall(meCoreHwMutexUnlock, 1))
#define meLibEmitSoftwareInterrupt()      (kcall(meCoreEmitSoftwareInterrupt, 1))
#define meLibOverclock()                  (kcall(meLibExtendedOverclock, 1))
#define meLibCancelOverclock()            (kcall(meLibExtendedCancelOverclock, 1))

#define meLibSetSharedUncached32(size)    meLibSetSharedUncachedMem(size, u32Me)

#define meLibSetSharedUncachedMem(size, type) \
  static volatile type _meLibSharedMemory[(size)] __attribute__((aligned(64), section(".uncached"))) = {(type)0}; \
  volatile type* const meLibSharedMemory __attribute__((aligned(64), section(".uncached"))) = \
    (volatile type*)(UNCACHED_USER_MASK | (type)_meLibSharedMemory)

#define meLibMakeUncachedMem(name, size, type) \
  volatile type name[(size)] __attribute__((aligned(64), section(".uncached"))) = {(type)0};

#define meLibMakeMemSegVar(name, mask, type) ((volatile type* const)((mask) | (type)name))

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
  hw(0xBCC00010) = 1;
  while (hw(0xBCC00010)) {
    meLibSync();
  };
  hw(0xBCC00070) = 0;
  hw(0xBCC00020) = -1;
  hw(0xBCC00030) = 1;
  hw(0xBCC00040) = 2; // 1
  meLibSync();
}

extern char __start__me_section;
extern char __stop__me_section;
extern u32Me SC_HW_RESET;

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
  void meLibExceptionHandlerInit(const u8 ip7);
  void meLibGetUncached32(volatile u32Me** const mem, const u32Me size);
#ifdef __cplusplus
}
#endif


#define ME_LIB_SETUP_DEFAULT_SUSPEND_HANDLER() \
extern "C" __attribute__((noinline, aligned(4))) \
void meLibOnExternalInterrupt(void) { \
  asm volatile( \
    ".set push                       \n" \
    ".set noreorder                  \n" \
    ".set noat                       \n" \
    \
    /* save regs context */ \
    "addi     $sp, $sp, -16          \n" \
    "sw       $k0, 0($sp)            \n" \
    "sw       $k1, 4($sp)            \n" \
    "sw       $ra, 8($sp)            \n" \
    "sw       $at, 12($sp)           \n" \
    \
    /* if SRAM_SHARED_VAR_0 equal 1 */   \
    "li       $k0, %2                \n" \
    "lw       $k1, 0($k0)            \n" \
    "li       $k0, 1                 \n" \
    "bne      $k1, $k0, 1f           \n" \
    "nop                             \n" \
    \
    /* call meLibSaveContext */ \
    "la       $k0, %0                \n" \
    "li       $k1, 0x80000000        \n" \
    "or       $k0, $k0, $k1          \n" \
    "cache    0x8, 0($k0)            \n" \
    "sync                            \n" \
    "jal      $k0                    \n" \
    "nop                             \n" \
    "1:                              \n" \
    \
    /* if SRAM_SHARED_VAR_0 equal 2 */ \
    "li       $k0, %2                \n" \
    "lw       $k1, 0($k0)            \n" \
    "li       $k0, 2                 \n" \
    "bne      $k1, $k0, 2f           \n" \
    "nop                             \n" \
    \
    /* call meLibRestoreContext */ \
    "la       $k0, %1                \n" \
    "li       $k1, 0x80000000        \n" \
    "or       $k0, $k0, $k1          \n" \
    "cache    0x8, 0($k0)            \n" \
    "sync                            \n" \
    "jal      $k0                    \n" \
    "nop                             \n" \
    "2:                              \n" \
    \
    /* reset SRAM_SHARED_VAR_0 */ \
    "li       $k0, %2                \n" \
    "sw       $zero, 0($k0)          \n" \
    "sync                            \n" \
    \
    /* restore regs context */ \
    "lw       $k0, 0($sp)            \n" \
    "lw       $k1, 4($sp)            \n" \
    "lw       $ra, 8($sp)            \n" \
    "lw       $at, 12($sp)           \n" \
    "addi     $sp, $sp, 16           \n" \
    \
    ".set pop                        \n" \
    : \
    :  "i" (meLibSaveContext), "i" (meLibRestoreContext), "i" (SRAM_SHARED_VAR_0) \
    : "k0", "k1", "memory" \
  ); \
} \
\
extern "C" void meLibOnSleep() { \
  SET_SRAM_SHARED_VAR(0, 1); \
  meCoreEmitSoftwareInterrupt(); \
  while (GET_SRAM_SHARED_VAR(0)) { \
    meLibSync(); \
  } \
  HW_SYS_RESET_ENABLE = 0x14; \
  meLibSync(); \
} \
\
extern "C" void meLibOnWake() { \
  SET_SRAM_SHARED_VAR(0, 3); \
  HW_SYS_RESET_ENABLE = 0x14; \
  HW_SYS_RESET_ENABLE = 0x00; \
  meLibSync(); \
  while (GET_SRAM_SHARED_VAR(0)) { \
    meLibSync(); \
  } \
  SET_SRAM_SHARED_VAR(0, 2); \
  meCoreEmitSoftwareInterrupt(); \
}

#endif

