#ifndef ME_VME_LIB_H
#define ME_VME_LIB_H

#define MECC_VME_LIB_VERSION      0xBe7a0004

#include "hw-registers.h"

#define vmeLibRefresh vmeLibFinish

#define VME_BASE_BUFFERS          0x44000000
#define VME_TOP_BUFFERS           0x44020000
#define VME_DATAPATH_BASE         0x440f8000

#define VME_PFX_ROUTE             0x84000000
#define VME_PFX_PARAM             0x00010000
#define VME_DEF_MAPPER            0x00003210
#define VME_PFX_END_TOKEN         0x00200000

// co-operand data source selectors
#define VME_TOP_0                 0x00000000
#define VME_TOP_1                 0x10000000
#define VME_TOP_2                 0x20000000
#define VME_TOP_3                 0x30000000
#define VME_BASE_0                0x40000000
#define VME_BASE_1                0x50000000
#define VME_BASE_2                0x60000000
#define VME_BASE_3                0x70000000

#define VME_BASE(index) (hw(VME_BASE_ADDR + (index * 4)))

// PE functional unit 0 descriptors, describing a local MUX, the ALU/Logic/MAC, a Saturator and Shifter.
#define VME_PE_0_FUNCTIONAL_UNIT_PRIMARY       0   // 0x000 // VME_PE_0_TOP_DESCRIPTOR => VME_PE_0_FUNCTIONAL_UNIT_0
#define VME_PE_1_FUNCTIONAL_UNIT_PRIMARY       1   // 0x004
#define VME_PE_2_FUNCTIONAL_UNIT_PRIMARY       2   // 0x008
#define VME_PE_3_FUNCTIONAL_UNIT_PRIMARY       3   // 0x00c

// PE functional unit 1 descriptors, describing a local MUX, the ALU/Logic/MAC, a Saturator and Shifter.
#define VME_PE_0_FUNCTIONAL_UNIT_SECONDARY     4   // 0x010 // VME_PE_0_BASE_DESCRIPTOR => VME_PE_0_FUNCTIONAL_UNIT_1
#define VME_PE_1_FUNCTIONAL_UNIT_SECONDARY     5   // 0x014
#define VME_PE_2_FUNCTIONAL_UNIT_SECONDARY     6   // 0x018
#define VME_PE_3_FUNCTIONAL_UNIT_SECONDARY     7   // 0x01c

// PE FU0 Operation registers/operands
#define VME_PE_0_FU_PRIMARY_REGISTER_A         8   // 0x020 // VME_PE_0_TOP_REGISTER_A => VME_PE_0_FU_0_REGISTER_A, RA
#define VME_PE_0_FU_PRIMARY_REGISTER_B         9   // 0x024 // VME_PE_0_TOP_REGISTER_B => VME_PE_0_FU_0_REGISTER_B, RB
#define VME_PE_1_FU_PRIMARY_REGISTER_A         10  // 0x028
#define VME_PE_1_FU_PRIMARY_REGISTER_B         11  // 0x02c
#define VME_PE_2_FU_PRIMARY_REGISTER_A         12  // 0x030
#define VME_PE_2_FU_PRIMARY_REGISTER_B         13  // 0x034
#define VME_PE_3_FU_PRIMARY_REGISTER_A         14  // 0x038
#define VME_PE_3_FU_PRIMARY_REGISTER_B         15  // 0x03c

// PE FU1 Operation registers/operands
#define VME_PE_0_FU_SECONDARY_REGISTER_A       16  // 0x040
#define VME_PE_0_FU_SECONDARY_REGISTER_B       17  // 0x044
#define VME_PE_1_FU_SECONDARY_REGISTER_A       18  // 0x048
#define VME_PE_1_FU_SECONDARY_REGISTER_B       19  // 0x04c
#define VME_PE_2_FU_SECONDARY_REGISTER_A       20  // 0x050
#define VME_PE_2_FU_SECONDARY_REGISTER_B       21  // 0x054
#define VME_PE_3_FU_SECONDARY_REGISTER_A       22  // 0x058
#define VME_PE_3_FU_SECONDARY_REGISTER_B       23  // 0x05c

// modifiers? (unclear)
#define VME_MODIFIER_A          24  // 0x060
#define VME_MODIFIER_B          25  // 0x064
#define VME_MODIFIER_C          26  // 0x068

// functional units 1 register hosting enable bits (not fully clear)
#define VME_ENABLE_FU_1         27  // 0x06c

// global interconnect
#define VME_INTERCONNECT_INPUT  28  // 0x070 // input paths, mapping source buffers to PE
#define VME_INTERCONNECT_FLOW   29  // 0x074 // inter-PE routing register. Each nibble selects the source flow PE index for a target PE lane.
#define VME_INTERCONNECT_ARCH   30  // 0x078 // inter-PE routing register. Each nibble selects the source arch PE index for a target PE lane.
#define VME_INTERCONNECT_UNK    31  // 0x07c // unknown
#define VME_INTERCONNECT_SKEW   32  // 0x080 // applies a cycle skew to the data stream forwarded to the target PE.


/*
 * PE0 AGUs
 * */
 
// PE0 AGU Read Top
#define VME_PE_0_READ_TOP_MODE         33  // 0x084
#define VME_PE_0_READ_TOP_COUNT        34  // 0x088
#define VME_PE_0_READ_TOP_INNER_0      35  // 0x08c
#define VME_PE_0_READ_TOP_INNER_1      36  // 0x090
#define VME_PE_0_READ_TOP_FORMAT_0     37  // 0x094
#define VME_PE_0_READ_TOP_FORMAT_1     38  // 0x098

// PE0 AGU Read Base
#define VME_PE_0_READ_BASE_MODE        39  // 0x09c
#define VME_PE_0_READ_BASE_COUNT       40  // 0x0a0
#define VME_PE_0_READ_BASE_INNER_0     41  // 0x0a4
#define VME_PE_0_READ_BASE_INNER_1     42  // 0x0a8
#define VME_PE_0_READ_BASE_FORMAT_0    43  // 0x0ac
#define VME_PE_0_READ_BASE_FORMAT_1    44  // 0x0b0

// PE0 AGU Write
#define VME_PE_0_WRITE_MODE            45  // 0x0b4
#define VME_PE_0_WRITE_COUNT           46  // 0x0b8
#define VME_PE_0_WRITE_INNER_0         47  // 0x0bc
#define VME_PE_0_WRITE_INNER_1         48  // 0x0c0
#define VME_PE_0_WRITE_FORMAT_0        49  // 0x0c4
#define VME_PE_0_WRITE_FORMAT_1        50  // 0x0c8


/*
 * PE1 AGUs
 * */
 
// PE1 AGU Read Top
#define VME_PE_1_READ_TOP_MODE         51  // 0x0cc
#define VME_PE_1_READ_TOP_COUNT        52  // 0x0d0
#define VME_PE_1_READ_TOP_INNER_0      53  // 0x0d4
#define VME_PE_1_READ_TOP_INNER_1      54  // 0x0d8
#define VME_PE_1_READ_TOP_FORMAT_0     55  // 0x0dc
#define VME_PE_1_READ_TOP_FORMAT_1     56  // 0x0e0

// PE1 AGU Read Base
#define VME_PE_1_READ_BASE_MODE        57  // 0x0e4
#define VME_PE_1_READ_BASE_COUNT       58  // 0x0e8
#define VME_PE_1_READ_BASE_INNER_0     59  // 0x0ec
#define VME_PE_1_READ_BASE_INNER_1     60  // 0x0f0
#define VME_PE_1_READ_BASE_FORMAT_0    61  // 0x0f4
#define VME_PE_1_READ_BASE_FORMAT_1    62  // 0x0f8

// PE1 AGU Write
#define VME_PE_1_WRITE_MODE            63  // 0x0fc
#define VME_PE_1_WRITE_COUNT           64  // 0x100
#define VME_PE_1_WRITE_INNER_0         65  // 0x104
#define VME_PE_1_WRITE_INNER_1         66  // 0x108
#define VME_PE_1_WRITE_FORMAT_2        67  // 0x10c
#define VME_PE_1_WRITE_FORMAT_3        68  // 0x110


/*
 * PE2 AGUs
 * */
 
// PE2 AGU Read Top
#define VME_PE_2_READ_TOP_MODE         69  // 0x114
#define VME_PE_2_READ_TOP_COUNT        70  // 0x118
#define VME_PE_2_READ_TOP_INNER_0      71  // 0x11c
#define VME_PE_2_READ_TOP_INNER_1      72  // 0x120
#define VME_PE_2_READ_TOP_FORMAT_0     73  // 0x124
#define VME_PE_2_READ_TOP_FORMAT_1     74  // 0x128

// PE2 AGU Read Base
#define VME_PE_2_READ_BASE_MODE        75  // 0x12c
#define VME_PE_2_READ_BASE_COUNT       76  // 0x130
#define VME_PE_2_READ_BASE_INNER_0     77  // 0x134
#define VME_PE_2_READ_BASE_INNER_1     78  // 0x138
#define VME_PE_2_READ_BASE_FORMAT_0    79  // 0x13c
#define VME_PE_2_READ_BASE_FORMAT_1    80  // 0x140

// PE2 AGU Write
#define VME_PE_2_WRITE_MODE            81  // 0x144
#define VME_PE_2_WRITE_COUNT           82  // 0x148
#define VME_PE_2_WRITE_INNER_0         83  // 0x14c
#define VME_PE_2_WRITE_INNER_1         84  // 0x150
#define VME_PE_2_WRITE_FORMAT_2        85  // 0x154
#define VME_PE_2_WRITE_FORMAT_3        86  // 0x158

/*
 * PE2 AGUs
 * */
 
// PE3 AGU Read Top
#define VME_PE_3_READ_TOP_MODE         87  // 0x15c
#define VME_PE_3_READ_TOP_COUNT        88  // 0x160
#define VME_PE_3_READ_TOP_INNER_0      89  // 0x164
#define VME_PE_3_READ_TOP_INNER_1      90  // 0x168
#define VME_PE_3_READ_TOP_FORMAT_2     91  // 0x16c
#define VME_PE_3_READ_TOP_FORMAT_3     92  // 0x170

// base buffer source
#define VME_PE_3_READ_BASE_MODE        93  // 0x174
#define VME_PE_3_READ_BASE_COUNT       94  // 0x178
#define VME_PE_3_READ_BASE_INNER_0     95  // 0x17c
#define VME_PE_3_READ_BASE_INNER_1     96  // 0x180
#define VME_PE_3_READ_BASE_FORMAT_2    97  // 0x184
#define VME_PE_3_READ_BASE_FORMAT_3    98  // 0x188

// destination
#define VME_PE_3_WRITE_MODE            99  // 0x18c
#define VME_PE_3_WRITE_COUNT           100 // 0x190
#define VME_PE_3_WRITE_INNER_0         101 // 0x194
#define VME_PE_3_WRITE_INNER_1         102 // 0x198
#define VME_PE_3_WRITE_FORMAT_2        103 // 0x19c
#define VME_PE_3_WRITE_FORMAT_3        104 // 0x1a0

// End ?
#define VME_UNKNOWN_105                105 // 0x1a4


// PE processing flow modes
#define VME_UNKNOWN_0   0
#define VME_UNKNOWN_1   1
#define VME_UNKNOWN_2   2
#define VME_DEFAULT     3
#define VME_UNKNOWN_4   4
#define VME_UNKNOWN_5   5
#define VME_UNKNOWN_6   6
#define VME_UNKNOWN_7   7
#define VME_UNKNOWN_8   8

#define vmeLibStart() \
{                     \
  volatile u32 VME_BASE_ADDR = VME_DATAPATH_BASE; \
  _vmeLibStart();
  

#define vmeLibFinish() \
  _vmeLibFinish();     \
}

#define vme_set_base(n) VME_BASE_ADDR = ((0x40000000 | n));

#define _vme_set_n(regIdx, val) ((VME_BASE(regIdx)) = (val))

#define vme_set_1(n, NAME, a)                       _vme_set_n(VME_##n##_##NAME, (a))
#define vme_set_2(n, NAME, a, b)                    _vme_set_n(VME_##n##_##NAME, (a)|(b))
#define vme_set_3(n, NAME, a, b, c)                 _vme_set_n(VME_##n##_##NAME, (a)|(b)|(c))
#define vme_set_4(n, NAME, a, b, c, d)              _vme_set_n(VME_##n##_##NAME, (a)|(b)|(c)|(d))
#define vme_set_5(n, NAME, a, b, c, d, e)           _vme_set_n(VME_##n##_##NAME, (a)|(b)|(c)|(d)|(e))
#define vme_set_6(n, NAME, a, b, c, d, e, f)        _vme_set_n(VME_##n##_##NAME, (a)|(b)|(c)|(d)|(e)|(f))
#define vme_set_7(n, NAME, a, b, c, d, e, f, g)     _vme_set_n(VME_##n##_##NAME, (a)|(b)|(c)|(d)|(e)|(f)|(g))
#define vme_set_8(n, NAME, a, b, c, d, e, f, g, h)  _vme_set_n(VME_##n##_##NAME, (a)|(b)|(c)|(d)|(e)|(f)|(g)|(h))

#define _vme_set(_1,_2,_3,_4,_5,_6,_7,_8, n, ...) n
#define vme_set(index, NAME, ...) \
  _vme_set(__VA_ARGS__, vme_set_8, vme_set_7, vme_set_6, vme_set_5, \
  vme_set_4, vme_set_3, vme_set_2, vme_set_1)(index, NAME, __VA_ARGS__)

// interconnect
#define vme_icn(NAME, ...) vme_set(INTERCONNECT, NAME, __VA_ARGS__)

// functionnal unit
#define vme_fu(TYPE) FUNCTIONAL_UNIT_##TYPE

// register
#define fu_reg(TYPE, REG) FU##_##TYPE##_REGISTER_##REG

// modifier
#define vme_mod(NAME, ...) vme_set(MODIFIER, NAME, __VA_ARGS__)

// address generation unit, generic
#define _vme_agu(_1, _2, _3, NAME, ...) NAME
#define vme_agu(...) _vme_agu(__VA_ARGS__, vme_agu_3, vme_agu_2, vme_agu_1)(__VA_ARGS__)
#define vme_agu_3(MODE, TARGET, PARAM)  MODE##_##TARGET##_##PARAM
#define vme_agu_2(MODE, PARAM)          MODE##_##PARAM

// process elements
#define vme_pe0(NAME, ...)  vme_set(PE_0, NAME, __VA_ARGS__)
#define vme_pe1(NAME, ...)  vme_set(PE_1, NAME, __VA_ARGS__)
#define vme_pe2(NAME, ...)  vme_set(PE_2, NAME, __VA_ARGS__)
#define vme_pe3(NAME, ...)  vme_set(PE_3, NAME, __VA_ARGS__)

// address generation unit
/*
#define vme_agu0(MODE, PARAM, ...)  \
  vme_pe0(vme_agu(MODE, PARAM), __VA_ARGS__)
#define vme_agu1(MODE, PARAM, ...)  \
  vme_pe1(vme_agu(MODE, PARAM), __VA_ARGS__)
#define vme_agu2(MODE, PARAM, ...)  \
  vme_pe2(vme_agu(MODE, PARAM), __VA_ARGS__)
#define vme_agu3(MODE, PARAM, ...)  \
  vme_pe3(vme_agu(MODE, PARAM), __VA_ARGS__)
*/

#define agu_top(PARAM)  vme_agu(READ, TOP, PARAM)
#define agu_base(PARAM)  vme_agu(READ, BASE, PARAM)
#define agu_write(PARAM)  vme_agu(WRITE, PARAM)

#ifdef __cplusplus
extern "C" {
#endif

void vmeLibEnable();
void vmeLibDisable();
void vmeLibWipe();

void vmeLibConfigTransfer(const int status);
void vmeLibSendCustomContext(void* context);
void vmeLibClearLocalBuffer(const int dst, const int count);

void _vmeLibStart();
void _vmeLibFinish();

void vmeLibSetFlow(const int mode);
void vmeLibTrigger();

#ifdef __cplusplus
}
#endif

#endif

