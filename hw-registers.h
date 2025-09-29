#ifndef HW_REGISTERS_H
#define HW_REGISTERS_H

#define u32Me                             unsigned int

#define hwp                               volatile u32Me*
#define hw(addr)                          (*((hwp)(addr)))
#define uhw(addr)                         ((u32Me*)(0x40000000 | ((u32Me)addr)))

#define HW_MEM_PROTECT_KERNEL_LOW_0       hw(0xbc000000)
#define HW_MEM_PROTECT_KERNEL_LOW_1       hw(0xbc000004)
#define HW_MEM_PROTECT_KERNEL_HIGH_0      hw(0xbc000008)
#define HW_MEM_PROTECT_KERNEL_HIGH_1      hw(0xbc00000C)
#define HW_SYS_RESET_ENABLE               hw(0xbc10004c)
#define HW_SYS_BUS_CLOCK_ENABLE           hw(0xbc100050)
#define HW_SYS_NMI_FLAGS                  hw(0xbc100004)
#define HW_SYS_TACHYON_CONFIG_STATUS      hw(0xbc100040)
#define HW_SYS_AVC_POWER                  hw(0xBC100070)


#define hwCacheHitWriteback(addr) \
  asm volatile( \
    ".set push                       \n" \
    ".set noreorder                  \n" \
    "la       $k0, %0                \n" \
    "cache    0x1a, 0($k0)           \n" \
    "sync                            \n" \
    ".set pop                        \n" \
    : \
    : "i" (addr) \
    : "k0", "memory" \
  );

#define hwCacheHitInvalidate(addr) \
  asm volatile( \
    ".set push                       \n" \
    ".set noreorder                  \n" \
    "la       $k0, %0                \n" \
    "cache    0x19, 0($k0)           \n" \
    "sync                            \n" \
    ".set pop                        \n" \
    : \
    : "i" (addr) \
    : "k0", "memory" \
  );
  
#define SRAM_SHARED_VARS_BASE             0xbfc00300/*0xbfc00700*/
#define SRAM_SHARED_VAR_0                 (SRAM_SHARED_VARS_BASE + 0)
#define SRAM_SHARED_VAR_1                 (SRAM_SHARED_VARS_BASE + 4)

#define GET_SRAM_SHARED_VAR(index)        hw(SRAM_SHARED_VARS_BASE + (index * 4))

#define SET_SRAM_SHARED_VAR(index, value) meLibSync(); \
                                          hw(SRAM_SHARED_VARS_BASE + (index * 4)) = (value); \
                                          meLibSync();

#endif
