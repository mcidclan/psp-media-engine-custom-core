#include "me-lib.h"

__attribute__((noinline, aligned(4)))
static void meLibexceptionHandler(void) {
meLibExceptionHandlerExternalInterrupt
  // todo
}

__attribute__((noinline, aligned(4)))
static void meLibexceptionHandler(void) {
  asm volatile(
      // clean status, disable interrupt
      "mtc0     $0, $12\n"
      "sync\n"

      // save k0 context
      "addi     $sp, $sp, -16\n"
      "sw       $k0, 0($sp)\n"
      "sw       $k1, 4($sp)\n"
      
      // check IP 2 on cause register and jump to the related handler if enabled 
      "mfc0     $k0, $13            \n"
      "andi     $k0, $k0, 0x400     \n"
      "beq      $k0, $zero, 1f      \n"
      "nop                          \n"
      "la       $k0, %0             \n"
      "li       $k1, 0x80000000     \n"
      "or       $k0, $k0, $k1       \n"
      "cache    0x8, 0($k0)         \n"
      "sync                         \n"
      "jr       $k0                 \n"
      "nop                          \n"
      "1:                           \n"

      // restore k0 context
      "lw       $k0, 0($sp)        \n"
      "lw       $k1, 4($sp)        \n"
      "addi     $sp, $sp, 16       \n"
      
      // exit
      "eret                        \n"
      
      // avoid potential pipeline issues
      "nop                         \n"
      "nop                         \n"
      "nop                         \n"
      "nop                         \n"
      "nop                         \n"
      "nop                         \n"
      "nop                         \n"
      :
      : "i" (meLibExceptionHandlerExternalInterrupt)
      : "k0", "k1", "memory"
  );
}

__attribute__((noinline, aligned(4)))
void meLibInitExceptions() {
  asm volatile(
    // clear status
    // "mtc0     $0, $12             \n"
    // "sync                         \n"    
    // setup exception handler
    "la       $k0, %0             \n"
    "li       $k1, 0x80000000     \n"
    "or       $k0, $k0, $k1       \n"
    "cache    0x8, 0($k0)         \n"
    "sync                         \n"
    // load exception handler
    "mtc0     $k0, $25            \n"
    "sync                         \n"
    // enable ME interrupt on system level
    "li       $k0, 0x80000000     \n"
    "sw       $k0, 0xbc300008($0) \n"
    "sync                         \n"
    // setup external interrupt on cp0 level
    "mfc0     $k0, $12            \n"
    "li       $k1, 0x401          \n"
    "or       $k0, $k0, $k1       \n"
    "mtc0     $k0, $12            \n"
    "sync\n"
    :
    : "i" (meLibexceptionHandler)
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
  memcpy((void *)ME_HANDLER_BASE, (void*)&__start__me_section, me_section_size);
  sceKernelDcacheWritebackInvalidateAll();
  HW_SYS_RESET_ENABLE = 0x04;
  HW_SYS_RESET_ENABLE = 0x00;
  meLibSync();
  return tableId;
}

#define PRX_FILE "./kcall.prx"
extern unsigned char kcall_module_start;
extern unsigned int kcall_module_size;

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

int meLibDefaultInit() {
  if(writePrx(&kcall_module_start, (int)&kcall_module_size) < 0) {
    return -3;
  }
  if (pspSdkLoadStartModule(PRX_FILE, PSP_MEMORY_PARTITION_KERNEL) < 0){
    sceKernelExitGame();
    return -3;
  }
  return kcall(meLibInit);
}

void meLibGetUncached32(volatile u32** const mem, const u32 size) {
  static void* _base = NULL;
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
  *mem = NULL;
  return;
}
