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

// PE top descriptors, describing a local MUX, the ALU/Logic/MAC, a Saturator and Shifter.
#define VME_PE_0_TOP_DESCRIPTOR    0   // 0x000 // PE 0
#define VME_PE_1_TOP_DESCRIPTOR    1   // 0x004 // PE 1
#define VME_PE_2_TOP_DESCRIPTOR    2   // 0x008 // PE 2
#define VME_PE_3_TOP_DESCRIPTOR    3   // 0x00c // PE 3

// PE base descriptors, describing a local MUX, the ALU/Logic/MAC, a Saturator and Shifter.
#define VME_PE_0_BASE_DESCRIPTOR   4   // 0x010 // PE 0
#define VME_PE_1_BASE_DESCRIPTOR   5   // 0x014 // PE 1
#define VME_PE_2_BASE_DESCRIPTOR   6   // 0x018 // PE 2
#define VME_PE_3_BASE_DESCRIPTOR   7   // 0x01c // PE 3

// DSP operation registers / operands for top src
// PE 0
#define VME_PE_0_TOP_REGISTER_A    8   // 0x020
#define VME_PE_0_TOP_REGISTER_B    9   // 0x024
// PE 1
#define VME_PE_1_TOP_REGISTER_A    10  // 0x028
#define VME_PE_1_TOP_REGISTER_B    11  // 0x02c
// PE 2
#define VME_PE_2_TOP_REGISTER_A    12  // 0x030
#define VME_PE_2_TOP_REGISTER_B    13  // 0x034
// PE 3
#define VME_PE_3_TOP_REGISTER_A    14  // 0x038
#define VME_PE_3_TOP_REGISTER_B    15  // 0x03c

// DSP operation registers / operands for base src
// PE 0
#define VME_PE_0_BASE_REGISTER_A   16  // 0x040
#define VME_PE_0_BASE_REGISTER_B   17  // 0x044
// PE 1
#define VME_PE_1_BASE_REGISTER_A   18  // 0x048
#define VME_PE_1_BASE_REGISTER_B   19  // 0x04c
// PE 2
#define VME_PE_2_BASE_REGISTER_A   20  // 0x050
#define VME_PE_2_BASE_REGISTER_B   21  // 0x054
// PE 3
#define VME_PE_3_BASE_REGISTER_A   22  // 0x058
#define VME_PE_3_BASE_REGISTER_B   23  // 0x05c

// mixer/mux ? (unclear)
#define VME_GLOBAL_MIXER_A         24  // 0x060
#define VME_GLOBAL_MIXER_B         25  // 0x064
#define VME_GLOBAL_MIXER_C         26  // 0x068

// base buffers feed base input and enable base descriptors
#define VME_GLOBAL_BASE_INPUT      27  // 0x06c

// crossbars
#define VME_GLOBAL_CROSSBAR_INPUT  28  // 0x070 // input paths, mapping source buffers to PE
#define VME_GLOBAL_CROSSBAR_FLOW   29  // 0x074 // inter-PE routing register. Each nibble selects the source arch PE index for a target PE lane.
#define VME_GLOBAL_CROSSBAR_ARCH   30  // 0x078 // inter-PE routing register. Each nibble selects the source flow PE index for a target PE lane.
#define VME_GLOBAL_CROSSBAR_UNK    31  // 0x07c // unknown
#define VME_GLOBAL_CROSSBAR_SKEW   32  // 0x080 // applies a cycle skew to the data stream forwarded to the target PE.

// PE 0
// top buffer source
#define VME_PE_0_TOP_SRC           33  // 0x084
#define VME_PE_0_TOP_COUNT         34  // 0x088
#define VME_PE_0_TOP_PARAM_0       35  // 0x08c
#define VME_PE_0_TOP_PARAM_1       36  // 0x090
#define VME_PE_0_TOP_PARAM_2       37  // 0x094
#define VME_PE_0_TOP_PARAM_3       38  // 0x098
// base buffer source
#define VME_PE_0_BASE_SRC          39  // 0x09c
#define VME_PE_0_BASE_COUNT        40  // 0x0a0
#define VME_PE_0_BASE_PARAM_0      41  // 0x0a4
#define VME_PE_0_BASE_PARAM_1      42  // 0x0a8
#define VME_PE_0_BASE_PARAM_2      43  // 0x0ac
#define VME_PE_0_BASE_PARAM_3      44  // 0x0b0
// destination
#define VME_PE_0_DST               45  // 0x0b4
#define VME_PE_0_DST_COUNT         46  // 0x0b8
#define VME_PE_0_DST_PARAM_0       47  // 0x0bc
#define VME_PE_0_DST_PARAM_1       48  // 0x0c0
#define VME_PE_0_DST_PARAM_2       49  // 0x0c4
#define VME_PE_0_DST_PARAM_3       50  // 0x0c8

// PE 1
// top buffer source
#define VME_PE_1_TOP_SRC           51  // 0x0cc
#define VME_PE_1_TOP_COUNT         52  // 0x0d0
#define VME_PE_1_TOP_PARAM_0       53  // 0x0d4
#define VME_PE_1_TOP_PARAM_1       54  // 0x0d8
#define VME_PE_1_TOP_PARAM_2       55  // 0x0dc
#define VME_PE_1_TOP_PARAM_3       56  // 0x0e0
// base buffer source
#define VME_PE_1_BASE_SRC          57  // 0x0e4
#define VME_PE_1_BASE_COUNT        58  // 0x0e8
#define VME_PE_1_BASE_PARAM_0      59  // 0x0ec
#define VME_PE_1_BASE_PARAM_1      60  // 0x0f0
#define VME_PE_1_BASE_PARAM_2      61  // 0x0f4
#define VME_PE_1_BASE_PARAM_3      62  // 0x0f8
// destination
#define VME_PE_1_DST               63  // 0x0fc
#define VME_PE_1_DST_COUNT         64  // 0x100
#define VME_PE_1_DST_PARAM_0       65  // 0x104
#define VME_PE_1_DST_PARAM_1       66  // 0x108
#define VME_PE_1_DST_PARAM_2       67  // 0x10c
#define VME_PE_1_DST_PARAM_3       68  // 0x110

// PE 2
// top buffer source
#define VME_PE_2_TOP_SRC           69  // 0x114
#define VME_PE_2_TOP_COUNT         70  // 0x118
#define VME_PE_2_TOP_PARAM_0       71  // 0x11c
#define VME_PE_2_TOP_PARAM_1       72  // 0x120
#define VME_PE_2_TOP_PARAM_2       73  // 0x124
#define VME_PE_2_TOP_PARAM_3       74  // 0x128
// base buffer source
#define VME_PE_2_BASE_SRC          75  // 0x12c
#define VME_PE_2_BASE_COUNT        76  // 0x130
#define VME_PE_2_BASE_PARAM_0      77  // 0x134
#define VME_PE_2_BASE_PARAM_1      78  // 0x138
#define VME_PE_2_BASE_PARAM_2      79  // 0x13c
#define VME_PE_2_BASE_PARAM_3      80  // 0x140
// destination
#define VME_PE_2_DST               81  // 0x144
#define VME_PE_2_DST_COUNT         82  // 0x148
#define VME_PE_2_DST_PARAM_0       83  // 0x14c
#define VME_PE_2_DST_PARAM_1       84  // 0x150
#define VME_PE_2_DST_PARAM_2       85  // 0x154
#define VME_PE_2_DST_PARAM_3       86  // 0x158

// PE 3
// top buffer source
#define VME_PE_3_TOP_SRC           87  // 0x15c
#define VME_PE_3_TOP_COUNT         88  // 0x160
#define VME_PE_3_TOP_PARAM_0       89  // 0x164
#define VME_PE_3_TOP_PARAM_1       90  // 0x168
#define VME_PE_3_TOP_PARAM_2       91  // 0x16c
#define VME_PE_3_TOP_PARAM_3       92  // 0x170
// base buffer source
#define VME_PE_3_BASE_SRC          93  // 0x174
#define VME_PE_3_BASE_COUNT        94  // 0x178
#define VME_PE_3_BASE_PARAM_0      95  // 0x17c
#define VME_PE_3_BASE_PARAM_1      96  // 0x180
#define VME_PE_3_BASE_PARAM_2      97  // 0x184
#define VME_PE_3_BASE_PARAM_3      98  // 0x188
// destination
#define VME_PE_3_DST               99  // 0x18c
#define VME_PE_3_DST_COUNT         100 // 0x190
#define VME_PE_3_DST_PARAM_0       101 // 0x194
#define VME_PE_3_DST_PARAM_1       102 // 0x198
#define VME_PE_3_DST_PARAM_2       103 // 0x19c
#define VME_PE_3_DST_PARAM_3       104 // 0x1a0

// End ?
#define VME_UNKNOWN_105            105 // 0x1a4


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

