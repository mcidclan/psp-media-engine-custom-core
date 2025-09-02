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
#define meLibDelayPipeline()              asm volatile("nop; nop; nop; nop; nop; nop; nop;")

inline void meLibHalt() {
  asm volatile(".word 0x70000000");
}

inline void meLibDcacheWritebackInvalidateAll() {
  asm volatile ("sync");
  for (int i = 0; i < 8192; i += 64) {
    asm("cache 0x14, 0(%0)" :: "r"(i));
    asm("cache 0x14, 0(%0)" :: "r"(i));
  }
  asm volatile ("sync");
}

inline void meLibGetUncached32(volatile u32** const mem, const u32 size) {
  static void* _base = nullptr;
  if (!_base) {
    const u32 byteCount = size * 4;
    _base = memalign(16, byteCount);
    memset(_base, 0, byteCount);
    sceKernelDcacheWritebackInvalidateAll();
    *mem = (u32*)(UNCACHED_USER_MASK | (u32)_base);
    __asm__ volatile (
      "cache 0x1b, 0(%0)  \n"
      "sync               \n"
      : : "r" (mem) : "memory"
    );
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
  HW_SYS_BUS_CLOCK_ENABLE      = 0x0f;
  HW_SYS_NMI_FLAGS             = 0xffffffff;
  HW_SYS_TACHYON_CONFIG_STATUS = 0x02;
  meLibSync();
    
  asm volatile(
    ".set noreorder                  \n"
    "li             $k0, 0x30000000  \n"
    "mtc0           $k0, $12         \n"
    "sync                            \n"

    "li             $k0, 0x279c637c  \n"
    "lw             $k1, 0x88300018  \n"
    "beq            $k0, $k1, 1f     \n"
    "nop                             \n"
    "li             $sp, 0x80200000  \n"
    "b              2f               \n"
    "nop                             \n"
    "1:                              \n"
    "li             $sp, 0x80400000  \n"
    "2:                              \n"

    "la             $k0, %0          \n"
    "li             $k1, 0x80000000  \n"
    "or             $k0, $k0, $k1    \n"
    
    "cache          0x8, 0($k0)      \n"
    "sync                            \n"
        
    "jr             $k0              \n"
    "nop                             \n"
    ".set reorder                    \n"
    :
    : "i" (meLibExec)
    : "k0", "k1", "memory"
  );
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
