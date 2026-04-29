#ifndef ME_VME_LIB_H
#define ME_VME_LIB_H

#include "hw-registers.h"

#define VME_BASE(index)   (0x440f8000 + (index * 4))

// descriptors
#define VME_DESCRIPTOR_0      0
#define VME_DESCRIPTOR_1      1
#define VME_DESCRIPTOR_2      2
#define VME_DESCRIPTOR_3      3

// unknown
#define VME_UNKNOWN_4         4
#define VME_UNKNOWN_5         5
#define VME_UNKNOWN_6         6
#define VME_UNKNOWN_7         7

// dsp registers
#define VME_REGISTER_A_0      8
#define VME_REGISTER_B_0      9
#define VME_REGISTER_A_1      10
#define VME_REGISTER_B_1      11
#define VME_REGISTER_A_2      12
#define VME_REGISTER_B_2      13
#define VME_REGISTER_A_3      14
#define VME_REGISTER_B_3      15

// unknown
#define VME_UNKNOWN_16        16
#define VME_UNKNOWN_17        17
#define VME_UNKNOWN_18        18
#define VME_UNKNOWN_19        19
#define VME_UNKNOWN_20        20
#define VME_UNKNOWN_21        21
#define VME_UNKNOWN_22        22
#define VME_UNKNOWN_23        23

// mixer
#define VME_MIXER_0           24
#define VME_MIXER_1           25
#define VME_MIXER_2           26

// mapper
#define VME_UNKNOWN_27        27
#define VME_MAPPER_1          28
#define VME_MAPPER_2          29
#define VME_MAPPER_3          30
#define VME_UNKNOWN_31        31
#define VME_UNKNOWN_32        32

// PE 0
// source I
#define VME_PE_0_I_SRC        33
#define VME_PE_0_I_COUNT      34
#define VME_PE_0_I_PARAM_0    35
#define VME_PE_0_I_PARAM_1    36
#define VME_PE_0_I_PARAM_2    37
#define VME_PE_0_I_PARAM_3    38
// source II
#define VME_PE_0_II_SRC       39
#define VME_PE_0_II_COUNT     40
#define VME_PE_0_II_PARAM_0   41
#define VME_PE_0_II_PARAM_1   42
#define VME_PE_0_II_PARAM_2   43
#define VME_PE_0_II_PARAM_3   44
// destination
#define VME_PE_0_DST          45
#define VME_PE_0_DST_COUNT    46
#define VME_PE_0_DST_PARAM_0  47
#define VME_PE_0_DST_PARAM_1  48
#define VME_PE_0_DST_PARAM_2  49
#define VME_PE_0_DST_PARAM_3  50

// PE 1
// source I
#define VME_PE_1_I_SRC        51
#define VME_PE_1_I_COUNT      52
#define VME_PE_1_I_PARAM_0    53
#define VME_PE_1_I_PARAM_1    54
#define VME_PE_1_I_PARAM_2    55
#define VME_PE_1_I_PARAM_3    56
// source II
#define VME_PE_1_II_SRC       57
#define VME_PE_1_II_COUNT     58
#define VME_PE_1_II_PARAM_0   59
#define VME_PE_1_II_PARAM_1   60
#define VME_PE_1_II_PARAM_2   61
#define VME_PE_1_II_PARAM_3   62
// destination
#define VME_PE_1_DST          63
#define VME_PE_1_DST_COUNT    64
#define VME_PE_1_DST_PARAM_0  65
#define VME_PE_1_DST_PARAM_1  66
#define VME_PE_1_DST_PARAM_2  67
#define VME_PE_1_DST_PARAM_3  68

// PE 2
// source I
#define VME_PE_2_I_SRC        69
#define VME_PE_2_I_COUNT      70
#define VME_PE_2_I_PARAM_0    71
#define VME_PE_2_I_PARAM_1    72
#define VME_PE_2_I_PARAM_2    73
#define VME_PE_2_I_PARAM_3    74
// source II
#define VME_PE_2_II_SRC       75
#define VME_PE_2_II_COUNT     76
#define VME_PE_2_II_PARAM_0   77
#define VME_PE_2_II_PARAM_1   78
#define VME_PE_2_II_PARAM_2   79
#define VME_PE_2_II_PARAM_3   80
// destination
#define VME_PE_2_DST          81
#define VME_PE_2_DST_COUNT    82
#define VME_PE_2_DST_PARAM_0  83
#define VME_PE_2_DST_PARAM_1  84
#define VME_PE_2_DST_PARAM_2  85
#define VME_PE_2_DST_PARAM_3  86

// PE 3
// source I
#define VME_PE_3_I_SRC        87
#define VME_PE_3_I_COUNT      88
#define VME_PE_3_I_PARAM_0    89
#define VME_PE_3_I_PARAM_1    90
#define VME_PE_3_I_PARAM_2    91
#define VME_PE_3_I_PARAM_3    92
// source II
#define VME_PE_3_II_SRC       93
#define VME_PE_3_II_COUNT     94
#define VME_PE_3_II_PARAM_0   95
#define VME_PE_3_II_PARAM_1   96
#define VME_PE_3_II_PARAM_2   97
#define VME_PE_3_II_PARAM_3   98
// destination
#define VME_PE_3_DST          99
#define VME_PE_3_DST_COUNT    100
#define VME_PE_3_DST_PARAM_0  101
#define VME_PE_3_DST_PARAM_1  102
#define VME_PE_3_DST_PARAM_2  103
#define VME_PE_3_DST_PARAM_3  104

// End ?
#define VME_UNKNOWN_105     105

#define vme_set(regIdx, val) ((VME_BASE(regIdx)) = (val))

#ifdef __cplusplus
extern "C" {
#endif

void vmeLibInit();
void vmeLibSendCustomBitstream(void* bitstream);
void vmeLibRefreshProcess();

#ifdef __cplusplus
}
#endif

#endif

