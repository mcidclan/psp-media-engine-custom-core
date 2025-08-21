#ifndef HW_REGISTERS_H
#define HW_REGISTERS_H

#define hwp                           volatile u32*
#define hw(addr)                      (*((hwp)(addr)))
#define uhw(addr)                     ((u32*)(0x40000000 | ((u32)addr)))

#define HW_SYS_RESET_ENABLE           hw(0xbc10004c)
#define HW_SYS_BUS_CLOCK_ENABLE       hw(0xbc100050)
#define HW_SYS_NMI_FLAGS              hw(0xbc100004)
#define HW_SYS_TACHYON_CONFIG_STATUS  hw(0xbc100040)

#endif

