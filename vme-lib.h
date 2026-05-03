#ifndef ME_VME_LIB_H
#define ME_VME_LIB_H

#include "hw-registers.h"

#define vmeLibRefresh vmeLibFinish

#define VME_BUFFERS_PRIMARY       0x44000000
#define VME_BUFFERS_SECONDARY     0x44020000
#define VME_DATAPATH_BASE         0x440f8000

#define VME_DEF_SRC               0x84000000
#define VME_DEF_DST               0x84060000
#define VME_DEF_PARAM             0x00010000
#define VME_DEF_MAPPER            0x00003210

#define VME_BASE(index) (hw(VME_DATAPATH_BASE + (index * 4)))

// descriptors
#define VME_0_DESCRIPTOR          0
#define VME_1_DESCRIPTOR          1
#define VME_2_DESCRIPTOR          2
#define VME_3_DESCRIPTOR          3

// unknown
#define VME_UNKNOWN_4             4
#define VME_UNKNOWN_5             5
#define VME_UNKNOWN_6             6
#define VME_UNKNOWN_7             7

// dsp registers
#define VME_0_REGISTER_A          8
#define VME_0_REGISTER_B          9

#define VME_1_REGISTER_A          10
#define VME_1_REGISTER_B          11

#define VME_2_REGISTER_A          12
#define VME_2_REGISTER_B          13

#define VME_3_REGISTER_A          14
#define VME_3_REGISTER_B          15

// unknown
#define VME_UNKNOWN_16            16
#define VME_UNKNOWN_17            17
#define VME_UNKNOWN_18            18
#define VME_UNKNOWN_19            19
#define VME_UNKNOWN_20            20
#define VME_UNKNOWN_21            21
#define VME_UNKNOWN_22            22
#define VME_UNKNOWN_23            23

// mixer (unclear)
#define VME_MIXER_A               24
#define VME_MIXER_B               25
#define VME_MIXER_C               26

// unknown
#define VME_UNKNOWN_27            27
#define VME_UNKNOWN_28            28

// mapper
#define VME_MAPPER_A              29
#define VME_MAPPER_B              30
#define VME_MAPPER_C              31

// bitfield (unclear)
#define VME_BITFIELD_DESCRIPTOR   32

// PE 0
// source I
#define VME_0_PRIMARY_SRC         33
#define VME_0_PRIMARY_COUNT       34
#define VME_0_PRIMARY_PARAM_0     35
#define VME_0_PRIMARY_PARAM_1     36
#define VME_0_PRIMARY_PARAM_2     37
#define VME_0_PRIMARY_PARAM_3     38
// source II
#define VME_0_SECONDARY_SRC       39
#define VME_0_SECONDARY_COUNT     40
#define VME_0_SECONDARY_PARAM_0   41
#define VME_0_SECONDARY_PARAM_1   42
#define VME_0_SECONDARY_PARAM_2   43
#define VME_0_SECONDARY_PARAM_3   44
// destination
#define VME_0_DESTINATION         45
#define VME_0_DESTINATION_COUNT   46
#define VME_0_DESTINATION_PARAM_0 47
#define VME_0_DESTINATION_PARAM_1 48
#define VME_0_DESTINATION_PARAM_2 49
#define VME_0_DESTINATION_PARAM_3 50

// PE 1
// source I
#define VME_1_PRIMARY_SRC         51
#define VME_1_PRIMARY_COUNT       52
#define VME_1_PRIMARY_PARAM_0     53
#define VME_1_PRIMARY_PARAM_1     54
#define VME_1_PRIMARY_PARAM_2     55
#define VME_1_PRIMARY_PARAM_3     56
// source II
#define VME_1_SECONDARY_SRC       57
#define VME_1_SECONDARY_COUNT     58
#define VME_1_SECONDARY_PARAM_0   59
#define VME_1_SECONDARY_PARAM_1   60
#define VME_1_SECONDARY_PARAM_2   61
#define VME_1_SECONDARY_PARAM_3   62
// destination
#define VME_1_DESTINATION         63
#define VME_1_DESTINATION_COUNT   64
#define VME_1_DESTINATION_PARAM_0 65
#define VME_1_DESTINATION_PARAM_1 66
#define VME_1_DESTINATION_PARAM_2 67
#define VME_1_DESTINATION_PARAM_3 68

// PE 2
// source I
#define VME_2_PRIMARY_SRC         69
#define VME_2_PRIMARY_COUNT       70
#define VME_2_PRIMARY_PARAM_0     71
#define VME_2_PRIMARY_PARAM_1     72
#define VME_2_PRIMARY_PARAM_2     73
#define VME_2_PRIMARY_PARAM_3     74
// source II
#define VME_2_SECONDARY_SRC       75
#define VME_2_SECONDARY_COUNT     76
#define VME_2_SECONDARY_PARAM_0   77
#define VME_2_SECONDARY_PARAM_1   78
#define VME_2_SECONDARY_PARAM_2   79
#define VME_2_SECONDARY_PARAM_3   80
// destination
#define VME_2_DESTINATION         81
#define VME_2_DESTINATION_COUNT   82
#define VME_2_DESTINATION_PARAM_0 83
#define VME_2_DESTINATION_PARAM_1 84
#define VME_2_DESTINATION_PARAM_2 85
#define VME_2_DESTINATION_PARAM_3 86

// PE 3
// source I
#define VME_3_PRIMARY_SRC         87
#define VME_3_PRIMARY_COUNT       88
#define VME_3_PRIMARY_PARAM_0     89
#define VME_3_PRIMARY_PARAM_1     90
#define VME_3_PRIMARY_PARAM_2     91
#define VME_3_PRIMARY_PARAM_3     92
// source II
#define VME_3_SECONDARY_SRC       93
#define VME_3_SECONDARY_COUNT     94
#define VME_3_SECONDARY_PARAM_0   95
#define VME_3_SECONDARY_PARAM_1   96
#define VME_3_SECONDARY_PARAM_2   97
#define VME_3_SECONDARY_PARAM_3   98
// destination
#define VME_3_DESTINATION         99
#define VME_3_DESTINATION_COUNT   100
#define VME_3_DESTINATION_PARAM_0 101
#define VME_3_DESTINATION_PARAM_1 102
#define VME_3_DESTINATION_PARAM_2 103
#define VME_3_DESTINATION_PARAM_3 104

// End ?
#define VME_UNKNOWN_105           105

#define vme_set(regIdx, val) ((VME_BASE(regIdx)) = (val))

#ifdef __cplusplus
extern "C" {
#endif

void vmeLibInit();
void vmeLibWipe();
void vmeLibSendCustomBitstream(void* bitstream);
void vmeLibStart();
void vmeLibFinish();

#ifdef __cplusplus
}
#endif

#endif

