#include "me-lib.h"
  
__attribute__((weak)) void meLibOnExternalInterrupt(void) {
}

__attribute__((weak)) void meLibOnException(void) {
}

__attribute__((noinline, aligned(4)))
int meLibSaveContext(void) {
  const u32 CTX_BASE = ME_HANDLER_BASE + 0xC00;
  asm volatile(
    ".set push                       \n"
    ".set noreorder                  \n"
    ".set noat                       \n"
    
    "sw     $k0, -4($sp)             \n"
    
    // load CTX_BASE into $k0
    "lui    $k0, %%hi(%0)             \n"
    "ori    $k0, $k0, %%lo(%0)        \n"
        
    // save k0 and k1 (optional)
    "sw     $k1, 128($k0)            \n"
    "lw     $k1, -4($sp)             \n"
    "sw     $k1, 124($k0)            \n"
    
    // save context
    "sw     $at, 0($k0)              \n"   // $1
    "sw     $v0, 4($k0)              \n"   // $2
    "sw     $v1, 8($k0)              \n"   // $3
    "sw     $a0, 12($k0)             \n"   // $4
    "sw     $a1, 16($k0)             \n"   // $5
    "sw     $a2, 20($k0)             \n"   // $6
    "sw     $a3, 24($k0)             \n"   // $7
    "sw     $t0, 28($k0)             \n"   // $8
    "sw     $t1, 32($k0)             \n"   // $9
    "sw     $t2, 36($k0)             \n"   // $10
    "sw     $t3, 40($k0)             \n"   // $11
    "sw     $t4, 44($k0)             \n"   // $12
    "sw     $t5, 48($k0)             \n"   // $13
    "sw     $t6, 52($k0)             \n"   // $14
    "sw     $t7, 56($k0)             \n"   // $15
    "sw     $s0, 60($k0)             \n"   // $16
    "sw     $s1, 64($k0)             \n"   // $17
    "sw     $s2, 68($k0)             \n"   // $18
    "sw     $s3, 72($k0)             \n"   // $19
    "sw     $s4, 76($k0)             \n"   // $20
    "sw     $s5, 80($k0)             \n"   // $21
    "sw     $s6, 84($k0)             \n"   // $22
    "sw     $s7, 88($k0)             \n"   // $23
    "sw     $t8, 92($k0)             \n"   // $24
    "sw     $t9, 96($k0)             \n"   // $25
    // skip $k0                            // $26
    // skip $k1                            // $27
    "sw     $gp, 100($k0)            \n"   // $28
    "sw     $sp, 104($k0)            \n"   // $29
    "sw     $fp, 108($k0)            \n"   // $30
    "sw     $ra, 112($k0)            \n"   // $31
    
    // save hi and lo
    "mflo   $k1                      \n"
    "sw     $k1, 116($k0)            \n"
    "mfhi   $k1                      \n"
    "sw     $k1, 120($k0)            \n"
    
    //todo:
    // save cp1 registers if cp1 is enabled
    "swc1     $f0, 132($k0)          \n"
    "swc1     $f1, 136($k0)          \n"
    "swc1     $f2, 140($k0)          \n"
    "swc1     $f3, 144($k0)          \n"
    "swc1     $f4, 148($k0)          \n"
    "swc1     $f5, 152($k0)          \n"
    "swc1     $f6, 156($k0)          \n"
    "swc1     $f7, 160($k0)          \n"
    "swc1     $f8, 164($k0)          \n"
    "swc1     $f9, 168($k0)          \n"
    "swc1     $f10, 172($k0)         \n"
    "swc1     $f11, 176($k0)         \n"
    "swc1     $f12, 180($k0)         \n"
    "swc1     $f13, 184($k0)         \n"
    "swc1     $f14, 188($k0)         \n"
    "swc1     $f15, 192($k0)         \n"
    "swc1     $f16, 196($k0)         \n"
    "swc1     $f17, 200($k0)         \n"
    "swc1     $f18, 204($k0)         \n"
    "swc1     $f19, 208($k0)         \n"
    "swc1     $f20, 212($k0)         \n"
    "swc1     $f21, 216($k0)         \n"
    "swc1     $f22, 220($k0)         \n"
    "swc1     $f23, 224($k0)         \n"
    "swc1     $f24, 228($k0)         \n"
    "swc1     $f25, 232($k0)         \n"
    "swc1     $f26, 236($k0)         \n"
    "swc1     $f27, 240($k0)         \n"
    "swc1     $f28, 244($k0)         \n"
    "swc1     $f29, 248($k0)         \n"
    "swc1     $f30, 252($k0)         \n"
    "swc1     $f31, 256($k0)         \n"
    "1:                              \n"
    
    // save status
    "mfc0     $k1, $12               \n"
    "sync                            \n"
    "sw       $k1, 260($k0)          \n"
    
    ".set pop                        \n"
    :
    : "i" (CTX_BASE)
    : "$k0", "$k1", "memory"
  );
  
  return 0;
}

__attribute__((noinline, aligned(4)))
int meLibRestoreContext(void) {
  const u32 CTX_BASE = ME_HANDLER_BASE + 0xC00;
    asm volatile(
    ".set push                       \n"
    ".set noreorder                  \n"
    ".set noat                       \n"
    
    // load CTX_BASE into $k0
    "lui    $k0, %%hi(%0)             \n"
    "ori    $k0, $k0, %%lo(%0)        \n"
    
    // restore context
    "lw     $at, 0($k0)              \n"   // $1
    "lw     $v0, 4($k0)              \n"   // $2
    "lw     $v1, 8($k0)              \n"   // $3
    "lw     $a0, 12($k0)             \n"   // $4
    "lw     $a1, 16($k0)             \n"   // $5
    "lw     $a2, 20($k0)             \n"   // $6
    "lw     $a3, 24($k0)             \n"   // $7
    "lw     $t0, 28($k0)             \n"   // $8
    "lw     $t1, 32($k0)             \n"   // $9
    "lw     $t2, 36($k0)             \n"   // $10
    "lw     $t3, 40($k0)             \n"   // $11
    "lw     $t4, 44($k0)             \n"   // $12
    "lw     $t5, 48($k0)             \n"   // $13
    "lw     $t6, 52($k0)             \n"   // $14
    "lw     $t7, 56($k0)             \n"   // $15
    "lw     $s0, 60($k0)             \n"   // $16
    "lw     $s1, 64($k0)             \n"   // $17
    "lw     $s2, 68($k0)             \n"   // $18
    "lw     $s3, 72($k0)             \n"   // $19
    "lw     $s4, 76($k0)             \n"   // $20
    "lw     $s5, 80($k0)             \n"   // $21
    "lw     $s6, 84($k0)             \n"   // $22
    "lw     $s7, 88($k0)             \n"   // $23
    "lw     $t8, 92($k0)             \n"   // $24
    "lw     $t9, 96($k0)             \n"   // $25
    // skip $k0                            // $26
    // skip $k1                            // $27
    "lw     $gp, 100($k0)            \n"   // $28
    "lw     $sp, 104($k0)            \n"   // $29
    "lw     $fp, 108($k0)            \n"   // $30
    "lw     $ra, 112($k0)            \n"   // $31
    
    // restore hi and lo
    "lw     $k1, 116($k0)            \n"
    "mtlo   $k1                      \n"
    "lw     $k1, 120($k0)            \n"
    "mthi   $k1                      \n"
    
    // restore status
    "lw       $k1, 260($k0)          \n"
    "mtc0     $k1, $12               \n"
    "sync                            \n"
    
    //todo:
    // restore cp1 registers if cp1 is enabled
    "lwc1     $f0, 132($k0)          \n"
    "lwc1     $f1, 136($k0)          \n"
    "lwc1     $f2, 140($k0)          \n"
    "lwc1     $f3, 144($k0)          \n"
    "lwc1     $f4, 148($k0)          \n"
    "lwc1     $f5, 152($k0)          \n"
    "lwc1     $f6, 156($k0)          \n"
    "lwc1     $f7, 160($k0)          \n"
    "lwc1     $f8, 164($k0)          \n"
    "lwc1     $f9, 168($k0)          \n"
    "lwc1     $f10, 172($k0)         \n"
    "lwc1     $f11, 176($k0)         \n"
    "lwc1     $f12, 180($k0)         \n"
    "lwc1     $f13, 184($k0)         \n"
    "lwc1     $f14, 188($k0)         \n"
    "lwc1     $f15, 192($k0)         \n"
    "lwc1     $f16, 196($k0)         \n"
    "lwc1     $f17, 200($k0)         \n"
    "lwc1     $f18, 204($k0)         \n"
    "lwc1     $f19, 208($k0)         \n"
    "lwc1     $f20, 212($k0)         \n"
    "lwc1     $f21, 216($k0)         \n"
    "lwc1     $f22, 220($k0)         \n"
    "lwc1     $f23, 224($k0)         \n"
    "lwc1     $f24, 228($k0)         \n"
    "lwc1     $f25, 232($k0)         \n"
    "lwc1     $f26, 236($k0)         \n"
    "lwc1     $f27, 240($k0)         \n"
    "lwc1     $f28, 244($k0)         \n"
    "lwc1     $f29, 248($k0)         \n"
    "lwc1     $f30, 252($k0)         \n"
    "lwc1     $f31, 256($k0)         \n"
    "1:                              \n"
    
    // restore k0 and k1 (optional)
    "lw     $k1, 128($k0)            \n"
    "lw     $k0, 124($k0)            \n"
    
    ".set pop                        \n"
    :
    : "i" (CTX_BASE)
    : "memory"
  );
  
  return 0;
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
    
    // call meLibOnException
    "la       $k0, %1                \n"
    "li       $k1, 0x80000000        \n"
    "or       $k0, $k0, $k1          \n"
    "cache    0x8, 0($k0)            \n"
    "sync                            \n"
    "jal      $k0                    \n"
    "nop                             \n"
    
    // check DB
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
    // check IP 2 on cause register
    "mfc0     $k0, $13               \n"
    "andi     $k0, $k0, 0x400        \n"
    "beq      $k0, $zero, 1f         \n"
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
    : "i" (meLibExceptionHandleExternalInterrupt), "i" (meLibOnException)
    : "k0", "k1", "memory"
  );
}

__attribute__((noinline, aligned(4)))
void meLibExceptionHandlerInit() {
  asm volatile(
    ".set noreorder                  \n"
    // setup exception handler
    "la       $k0, %0                \n"
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
    // setup external interrupt on cp0 level
    "mfc0     $k0, $12               \n"
    "li       $k1, 0x401             \n"
    "or       $k0, $k0, $k1          \n"
    "mtc0     $k0, $12               \n"
    "sync                            \n"
    ".set reorder                    \n"
    :
    : "i" (meLibExceptionHandler)
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
