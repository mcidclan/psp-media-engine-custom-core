// WIP, do not include this files
#ifndef VME_FU_3N_OPCODE
#define VME_FU_3N_OPCODE

#define VME_FU_3N_OPCODE_000                       0x000 
#define VME_FU_3N_OPCODE_ADDI                      0x010 /* (back[n] + front[n]) >> k */
#define VME_FU_3N_OPCODE_020                       0x020 /* (back[n] + front[n]) >> k */
#define VME_FU_3N_OPCODE_ADDI_ADD_A                0x030 /* (back[n] + front[n]) + a  */
#define VME_FU_3N_OPCODE_ADDI_FRONT_SHIFT_B        0x040 /* back[n] + (front[n] >> b) */
#define VME_FU_3N_OPCODE_SUBI_FRONT_SHIFT_B        0x050 /* back[n] - (front[n] >> b) */
#define VME_FU_3N_OPCODE_PREDICATED_BACK_NEGATE    0x060 /* (front[n] & a) ? -back[n] : back[n]*/
#define VME_FU_3N_OPCODE_070                       0x070 /* (back[n] << k) + b */
#define VME_FU_3N_OPCODE_080                       0x080
#define VME_FU_3N_OPCODE_090                       0x090
#define VME_FU_3N_OPCODE_0a0                       0x0a0
#define VME_FU_3N_OPCODE_0b0                       0x0b0
#define VME_FU_3N_OPCODE_0c0                       0x0c0
#define VME_FU_3N_0PCODE_0d0                       0x0d0
#define VME_FU_3N_OPCODE_0e0                       0x0e0
#define VME_FU_3N_OPCODE_0f0                       0x0f0

//...

#define VME_FU_3N_OPCODE_200                       0x200
#define VME_FU_3N_OPCODE_210                       0x210
#define VME_FU_3N_OPCODE_220                       0x220
#define VME_FU_3N_OPCODE_230                       0x230
#define VME_FU_3N_OPCODE_240                       0x240
#define VME_FU_3N_OPCODE_250                       0x250
#define VME_FU_3N_OPCODE_260                       0x260
#define VME_FU_3N_OPCODE_270                       0x270
#define VME_FU_3N_OPCODE_280                       0x280
#define VME_FU_3N_OPCODE_290                       0x290
#define VME_FU_3N_OPCODE_2a0                       0x2a0
#define VME_FU_3N_OPCODE_2b0                       0x2b0
#define VME_FU_3N_OPCODE_2c0                       0x2c0
#define VME_FU_3N_OPCODE_2d0                       0x2d0
#define VME_FU_3N_OPCODE_2e0                       0x2e0
#define VME_FU_3N_OPCODE_2f0                       0x2f0

#endif
