#include "me-lib.h"

__attribute__((section("_me_section")))
void meLibHandler() {  
  HW_SYS_BUS_CLOCK_ENABLE      = 0x0f;
  HW_SYS_TACHYON_CONFIG_STATUS |= 0x02;
  HW_SYS_NMI_FLAGS             = 0xffffffff;
  meLibSync();
  
  meLibUnlockHwUserRegisters();
  meLibUnlockMemory();
  meLibSetMinimalVmeConfig();
  
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
    : "i" (meLibOnProcess)
    : "k0", "k1", "memory"
  );
}
