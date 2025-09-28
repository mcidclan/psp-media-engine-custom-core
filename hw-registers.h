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
#define HW_SRAM_SHARED_VAR0               hw(0xbfc00300)

#endif
