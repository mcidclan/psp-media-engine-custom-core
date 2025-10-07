#include "me-lib.h"
  
__attribute__((weak, noinline, aligned(4)))
void meLibOnExternalInterrupt(void) {
}

__attribute__((weak, noinline, aligned(4)))
void meLibOnInternalTimerInterrupt(void) {
  // todo: default implementation
}

__attribute__((weak, noinline, aligned(4)))
void meLibOnException(void) {
}

__attribute__((weak, noinline, aligned(4)))
void meLibOnSleep(void) {
}

__attribute__((weak, noinline, aligned(4)))
void meLibOnWake(void) {
}

__attribute__((noinline, aligned(4)))
static void meLibExceptionHandleExternalInterrupt(void) {
  asm volatile(  
    ".set noreorder                  \n"
    // save $ra
    "addi     $sp, $sp, -8           \n"
    "sw       $ra, 0($sp)            \n"
    // clear ME interrupt flag on system level
    "li       $k0, 0x80000000        \n"
    "sw       $k0, 0xbc300000($0)    \n"
    "sync                            \n"
    // wait for 0xbc300000 to be cleared (become 0)
    "1:\n"
    "lw      $k1, 0xbc300000($0)\n"
    "bne     $k1, $0, 1b\n"
    "nop\n"
    // call meLibOnExternalInterrupt
    "la       $k0, %0                \n"
    "li       $k1, 0x80000000        \n"
    "or       $k0, $k0, $k1          \n"
    "cache    0x8, 0($k0)            \n"
    "sync                            \n"
    "jal      $k0                    \n"
    "nop                             \n"
    // restore $ra
    "lw       $ra, 0($sp)            \n"
    "addi     $sp, $sp, 8            \n"
    ".set reorder                    \n"
    :
    : "i" (meLibOnExternalInterrupt)
    : "k0", "k1", "memory"
  );
}

__attribute__((noinline, aligned(4)))
static void meLibExceptionHandler(void) {
  asm volatile(
    ".set push                       \n"
    ".set noreorder                  \n"
    ".set noat                       \n"
    
    // save regs context
    "addi     $sp, $sp, -24          \n"
    "sw       $k0, 0($sp)            \n"
    "sw       $k1, 4($sp)            \n"
    "sw       $ra, 8($sp)            \n"
    "sw       $at, 16($sp)           \n"
    
    // clear EXL & ERL bits, save status and disable interrupt
    "mfc0     $k0, $12               \n"
    "li       $k1, 0xfffffff9        \n"
    "and      $k0, $k0, $k1          \n"
    "sw       $k0, 12($sp)           \n"
    "mtc0     $0, $12                \n"
    "sync                            \n"
    
    // call meLibOnException (optional callback)
    "la       $k0, %1                \n"
    "li       $k1, 0x80000000        \n"
    "or       $k0, $k0, $k1          \n"
    "cache    0x8, 0($k0)            \n"
    "sync                            \n"
    "jal      $k0                    \n"
    "nop                             \n"
    
    // check IP 7 on cause register, jump to related handler then update compare if enabled
    "mfc0    $k0, $13                \n"
    "andi    $k0, $k0, 0x8000        \n"
    "beqz    $k0, 3f                 \n"
    "nop                             \n"
    // jump to Internal Timer interrupt (IP 7)
    "la       $k0, %2                \n"
    "li       $k1, 0x80000000        \n"
    "or       $k0, $k0, $k1          \n"
    "cache    0x8, 0($k0)            \n"
    "sync                            \n"
    "jal      $k0                    \n"
    "nop                             \n"
    // update compare register
    "mfc0    $k1, $9                 \n"
    "li      $k0, 166500             \n"
    "addu    $k1, $k1, $k0           \n"
    "mtc0    $k1, $11                \n"
    "sync                            \n"
    
    "3:                              \n"
    
    // check DB on cause register, readjust epc if necessary
    "mfc0     $k0, $13               \n"
    "srl      $k1, $k0, 31           \n"
    "beqz     $k1, 2f                \n"
    "nop                             \n"
    // readjust epc (if it occured in a delay slot then replay the branching)
    "mfc0     $k1, $14               \n"
    "addiu    $k1, $k1, -4            \n"
    "mtc0     $k1, $14               \n"
    "sync                            \n"
    
    "2:                              \n"
    
    // check IP 2 on cause register, jump to related handler if enabled
    "mfc0     $k0, $13               \n"
    "andi     $k0, $k0, 0x400        \n"
    "beqz     $k0, 1f                \n"
    "nop                             \n"
    // check ME interrupt flag - unecessary
    /*
    "lw       $k0, 0xbc300000($0)    \n"
    "sync                            \n"
    "lui      $k1, 0x8000            \n"
    "and      $k0, $k0, $k1          \n"
    "beq      $k0, $zero, 1f         \n"
    "nop                             \n"
    */
    // jump to the related handler if IP 2 or ME interrupt flag is enabled 
    "la       $k0, %0                \n"
    "li       $k1, 0x80000000        \n"
    "or       $k0, $k0, $k1          \n"
    "cache    0x8, 0($k0)            \n"
    "sync                            \n"
    "jal      $k0                    \n"
    "nop                             \n"
    
    "1:                              \n"
    
    // restore status with external interrupt enabled
    "lw       $k0, 12($sp)            \n"
    "mtc0     $k0, $12               \n"
    "sync                            \n"
    // restore regs context
    "lw       $k0, 0($sp)            \n"
    "lw       $k1, 4($sp)            \n"
    "lw       $ra, 8($sp)            \n"
    "lw       $at, 16($sp)           \n"
    "addi     $sp, $sp, 24           \n"
    // avoid pipeline hazards
    "nop                             \n"
    "nop                             \n"
    // exit
    "eret                            \n"
    // avoid potential pipeline timing / flush issues
    "nop                             \n"
    "nop                             \n"
    "nop                             \n"
    "nop                             \n"
    "nop                             \n"
    "nop                             \n"
    "nop                             \n"
    ".set pop                        \n"
    :
    : "i" (meLibExceptionHandleExternalInterrupt), "i" (meLibOnException), "i" (meLibOnInternalTimerInterrupt)
    : "k0", "k1", "memory"
  );
}

__attribute__((noinline, aligned(4)))
void meLibExceptionHandlerInit(const u8 ip7) {
  u32 interrupts = 0x401;
  if (ip7) {
    interrupts = 0x8401;
  }
  SET_SRAM_SHARED_VAR(0, 0);
  asm volatile(
    ".set noreorder                  \n"
    // setup exception handler
    "la       $k0, %1                \n"
    "li       $k1, 0x80000000        \n"
    "or       $k0, $k0, $k1          \n"
    "cache    0x8, 0($k0)            \n"
    "sync                            \n"
    // load exception handler
    "mtc0     $k0, $25               \n"
    "sync                            \n"
    // enable ME interrupt on system level
    "li       $k0, 0x80000000        \n"
    "sw       $k0, 0xbc300008($0)    \n"
    "sync                            \n"
    // clear interrupts flag on system level
    "li       $k0, 0xffffffff        \n"
    "sw       $k0, 0xbc300000($0)    \n"
    "sync                            \n"

    "move     $k1, %0                \n"
    
    // check timer interrupt (IP 7) on status, init compare and count if enabled
    "andi     $k0, $k1, 0x8000       \n"
    "beqz     $k0, 1f                \n"
    "nop                             \n"
    // init compare and count registers
    "mtc0    $0, $9                  \n"
    "sync                            \n"
    "li      $k0, 166500              \n"
    "mtc0    $k0, $11                 \n"
    "sync                            \n"
    
    "1:                              \n"

    // setup external interrupt on cp0 level
    "mfc0     $k0, $12               \n"
//  "li       $k1, 0x401             \n"
    "or       $k0, $k0, $k1          \n"
    "mtc0     $k0, $12               \n"
    "sync                            \n"
    ".set reorder                    \n"
    :
    : "r"(interrupts), "i" (meLibExceptionHandler)
    : "k0", "k1", "memory"
  );
}

static inline int meLibInit() {
  const int tableId = meCoreGetTableIdFromWitnessWord();
  if (tableId < 2) {
    return -1;
  }
  meCoreSelectSystemTable(tableId);
  
  #define me_section_size (&__stop__me_section - &__start__me_section)
  memcpy((void*)ME_HANDLER_BASE, (void*)&__start__me_section, me_section_size);
  
  sceKernelDcacheWritebackInvalidateAll();
  sceKernelIcacheInvalidateAll();
  
  HW_SYS_RESET_ENABLE = 0x14;
  HW_SYS_RESET_ENABLE = 0x00;
  meLibSync();
  return tableId;
}

#define PRX_FILE "./kcall.prx"
extern unsigned char embedded_kcall[];
extern unsigned int embedded_kcall_len;

int writePrx(void* start, int size) {
  SceUID fd = sceIoOpen(PRX_FILE, PSP_O_WRONLY | PSP_O_CREAT, 0777);
  if (fd < 0) {
    return -1;
  }
  int _size = sceIoWrite(fd, start, size);
  sceIoClose(fd);
  if (_size != size) {
    return -1;
  }
  return 0;
}

int eventHandler(int eventId) {
  switch(eventId) {
    case 0x00004005:
      meLibOnSleep();
    break;
    case 0x00010005:
      meLibOnWake();
    break;
  }
  return 0;
}

int meLibDefaultInit() {
  if(writePrx(embedded_kcall, (int)embedded_kcall_len) < 0) {
    return -3;
  }
  if (pspSdkLoadStartModule(PRX_FILE, PSP_MEMORY_PARTITION_KERNEL) < 0){
    sceKernelExitGame();
    return -3;
  }
  if(kinit((void*)eventHandler) < 0) {
    return -3;
  };
  return kcall(meLibInit);
}

void meLibGetUncached32(volatile u32Me** const mem, const u32Me size) {
  static void* _base = NULL;
  if (!_base) {
    const u32Me byteCount = size * 4;
    _base = memalign(16, byteCount);
    memset(_base, 0, byteCount);
    sceKernelDcacheWritebackInvalidateAll();
    *mem = (u32Me*)(UNCACHED_USER_MASK | (u32Me)_base);
    __asm__ volatile (
      "cache 0x1b, 0(%0)  \n"
      "sync               \n"
      : : "r" (mem) : "memory"
    );
    return;
  } else if (!size) {
    free(_base);
  }
  *mem = NULL;
  return;
}
