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

// co-operand data source selectors
#define VME_TOP_0                 0x00000000
#define VME_TOP_1                 0x10000000
#define VME_TOP_2                 0x20000000
#define VME_TOP_3                 0x30000000
#define VME_BASE_0                0x40000000
#define VME_BASE_1                0x50000000
#define VME_BASE_2                0x60000000
#define VME_BASE_3                0x70000000

#define VME_BASE(index) (hw(VME_DATAPATH_BASE + (index * 4)))

// descriptors for top buffers processes
#define VME_PE_0_TOP_DESCRIPTOR    0 // PE 0
#define VME_PE_1_TOP_DESCRIPTOR    1 // PE 1
#define VME_PE_2_TOP_DESCRIPTOR    2 // PE 2
#define VME_PE_3_TOP_DESCRIPTOR    3 // PE 3

// descriptors for base buffers processes
#define VME_PE_0_BASE_DESCRIPTOR   4 // PE 0
#define VME_PE_1_BASE_DESCRIPTOR   5 // PE 1
#define VME_PE_2_BASE_DESCRIPTOR   6 // PE 2
#define VME_PE_3_BASE_DESCRIPTOR   7 // PE 3

// DSP operation registers for top src
// PE 0
#define VME_PE_0_TOP_REGISTER_A    8
#define VME_PE_0_TOP_REGISTER_B    9
// PE 1
#define VME_PE_1_TOP_REGISTER_A    10
#define VME_PE_1_TOP_REGISTER_B    11
// PE 2
#define VME_PE_2_TOP_REGISTER_A    12
#define VME_PE_2_TOP_REGISTER_B    13
// PE 3
#define VME_PE_3_TOP_REGISTER_A    14
#define VME_PE_3_TOP_REGISTER_B    15

// DSP operation registers for base src
// PE 0
#define VME_PE_0_BASE_REGISTER_A   16
#define VME_PE_0_BASE_REGISTER_B   17
// PE 1
#define VME_PE_1_BASE_REGISTER_A   18
#define VME_PE_1_BASE_REGISTER_B   19
// PE 2
#define VME_PE_2_BASE_REGISTER_A   20
#define VME_PE_2_BASE_REGISTER_B   21
// PE 3
#define VME_PE_3_BASE_REGISTER_A   22
#define VME_PE_3_BASE_REGISTER_B   23

// mixer (unclear)
#define VME_GLOBAL_MIXER_A         24
#define VME_GLOBAL_MIXER_B         25
#define VME_GLOBAL_MIXER_C         26

// base buffers feed base input and enable base descriptors
#define VME_GLOBAL_BASE_INPUT      27

// input index from which the pipeline starts
#define VME_GLOBAL_INPUT_INDEX     28

// mapper
#define VME_GLOBAL_MAPPER_A        29
#define VME_GLOBAL_MAPPER_B        30

// unclear / not sure
#define VME_GLOBAL_MAPPER_C        31

// bitfield (unclear)
#define VME_GLOBAL_BITFIELD        32

// PE 0
// top buffer source
#define VME_PE_0_TOP_SRC           33
#define VME_PE_0_TOP_COUNT         34
#define VME_PE_0_TOP_PARAM_0       35
#define VME_PE_0_TOP_PARAM_1       36
#define VME_PE_0_TOP_PARAM_2       37
#define VME_PE_0_TOP_PARAM_3       38
// base buffer source
#define VME_PE_0_BASE_SRC          39
#define VME_PE_0_BASE_COUNT        40
#define VME_PE_0_BASE_PARAM_0      41
#define VME_PE_0_BASE_PARAM_1      42
#define VME_PE_0_BASE_PARAM_2      43
#define VME_PE_0_BASE_PARAM_3      44
// destination
#define VME_PE_0_DST               45
#define VME_PE_0_DST_COUNT         46
#define VME_PE_0_DST_PARAM_0       47
#define VME_PE_0_DST_PARAM_1       48
#define VME_PE_0_DST_PARAM_2       49
#define VME_PE_0_DST_PARAM_3       50

// PE 1
// top buffer source
#define VME_PE_1_TOP_SRC           51
#define VME_PE_1_TOP_COUNT         52
#define VME_PE_1_TOP_PARAM_0       53
#define VME_PE_1_TOP_PARAM_1       54
#define VME_PE_1_TOP_PARAM_2       55
#define VME_PE_1_TOP_PARAM_3       56
// base buffer source
#define VME_PE_1_BASE_SRC          57
#define VME_PE_1_BASE_COUNT        58
#define VME_PE_1_BASE_PARAM_0      59
#define VME_PE_1_BASE_PARAM_1      60
#define VME_PE_1_BASE_PARAM_2      61
#define VME_PE_1_BASE_PARAM_3      62
// destination
#define VME_PE_1_DST               63
#define VME_PE_1_DST_COUNT         64
#define VME_PE_1_DST_PARAM_0       65
#define VME_PE_1_DST_PARAM_1       66
#define VME_PE_1_DST_PARAM_2       67
#define VME_PE_1_DST_PARAM_3       68

// PE 2
// top buffer source
#define VME_PE_2_TOP_SRC           69
#define VME_PE_2_TOP_COUNT         70
#define VME_PE_2_TOP_PARAM_0       71
#define VME_PE_2_TOP_PARAM_1       72
#define VME_PE_2_TOP_PARAM_2       73
#define VME_PE_2_TOP_PARAM_3       74
// base buffer source
#define VME_PE_2_BASE_SRC          75
#define VME_PE_2_BASE_COUNT        76
#define VME_PE_2_BASE_PARAM_0      77
#define VME_PE_2_BASE_PARAM_1      78
#define VME_PE_2_BASE_PARAM_2      79
#define VME_PE_2_BASE_PARAM_3      80
// destination
#define VME_PE_2_DST               81
#define VME_PE_2_DST_COUNT         82
#define VME_PE_2_DST_PARAM_0       83
#define VME_PE_2_DST_PARAM_1       84
#define VME_PE_2_DST_PARAM_2       85
#define VME_PE_2_DST_PARAM_3       86

// PE 3
// top buffer source
#define VME_PE_3_TOP_SRC           87
#define VME_PE_3_TOP_COUNT         88
#define VME_PE_3_TOP_PARAM_0       89
#define VME_PE_3_TOP_PARAM_1       90
#define VME_PE_3_TOP_PARAM_2       91
#define VME_PE_3_TOP_PARAM_3       92
// base buffer source
#define VME_PE_3_BASE_SRC          93
#define VME_PE_3_BASE_COUNT        94
#define VME_PE_3_BASE_PARAM_0      95
#define VME_PE_3_BASE_PARAM_1      96
#define VME_PE_3_BASE_PARAM_2      97
#define VME_PE_3_BASE_PARAM_3      98
// destination
#define VME_PE_3_DST               99
#define VME_PE_3_DST_COUNT         100
#define VME_PE_3_DST_PARAM_0       101
#define VME_PE_3_DST_PARAM_1       102
#define VME_PE_3_DST_PARAM_2       103
#define VME_PE_3_DST_PARAM_3       104

// End ?
#define VME_UNKNOWN_105            105

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
void vmeLibSendCustomBitstream(void* bitstream);
void vmeLibClearLocalBuffer(const int dst, const int count);
void vmeLibStart();
void vmeLibFinish();

#ifdef __cplusplus
}
#endif

#endif

