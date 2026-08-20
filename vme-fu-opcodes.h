// WIP, do not include this files
#ifndef VME_FU_3N_OPCODE
#define VME_FU_3N_OPCODE

#define op_sel(value) ((value) << 12)

#define OP_000 0x000 // zero
#define OP_000 0x001
#define OP_000 0x002
#define OP_000 0x003
#define OP_000 0x004 // back[n]
#define OP_000 0x005
#define OP_000 0x006
#define OP_000 0x007
#define OP_000 0x008 // back[n]
#define OP_000 0x009
#define OP_000 0x00a
#define OP_000 0x00b
#define OP_000 0x00c // b
#define OP_000 0x00d
#define OP_000 0x00e
#define OP_000 0x00f

#define OP_000 0x010 // (back[n] + front[n])
#define OP_000 0x011
#define OP_000 0x012
#define OP_000 0x013
#define OP_000 0x014 // back[n]
#define OP_000 0x015
#define OP_000 0x016
#define OP_000 0x017
#define OP_000 0x018 // (front[n] - back[n])
#define OP_000 0x019
#define OP_000 0x01a
#define OP_000 0x01b
#define OP_000 0x01c // back[n]
#define OP_000 0x01d
#define OP_000 0x01e
#define OP_000 0x01f

#define OP_000 0x030 // (back[n] + front[n]) + a
#define OP_000 0x031
#define OP_000 0x032
#define OP_000 0x033
#define OP_000 0x034 // (back[n] - front[n]) + a
#define OP_000 0x035
#define OP_000 0x036
#define OP_000 0x037
#define OP_000 0x038 // (front[n] + back[n])
#define OP_000 0x039
#define OP_000 0x03a
#define OP_000 0x03b
#define OP_000 0x03c // (front[n] - back[n])
#define OP_000 0x03d
#define OP_000 0x03e
#define OP_000 0x03f

#define OP_000 0x040 // back[n] + (front[n] >> b)
#define OP_000 0x041
#define OP_000 0x042
#define OP_000 0x043
#define OP_000 0x044 // (back[n] >> b) + a
#define OP_000 0x045
#define OP_000 0x046
#define OP_000 0x047
#define OP_000 0x048 // (front[n] - back[n]) + b
#define OP_000 0x049
#define OP_000 0x04a
#define OP_000 0x04b
#define OP_000 0x04c // (-(back[n] >> b)) + a
#define OP_000 0x04d
#define OP_000 0x04e
#define OP_000 0x04f

#define OP_000 0x050 // back[n] - (front[n] >> b)
#define OP_000 0x051
#define OP_000 0x052
#define OP_000 0x053
#define OP_000 0x054 // (back[n] >> b) - a 
#define OP_000 0x055
#define OP_000 0x056
#define OP_000 0x057
#define OP_000 0x058 // ((0xFF00 & front[n]) + (0xFF00 & back[n])) >> 8
#define OP_000 0x059
#define OP_000 0x05a
#define OP_000 0x05b
#define OP_000 0x05c // (0xFF & back[n]) + (0xFF & front[n])
#define OP_000 0x05d
#define OP_000 0x05e
#define OP_000 0x05f

#define OP_000 0x060 // (front[n] & a) ? -back[n] : back[n]
#define OP_000 0x061
#define OP_000 0x062
#define OP_000 0x063
#define OP_000 0x064 // (back[n] & a) != 0 ? x : NEG(back[n]) (~back[n] + 1) 
#define OP_000 0x065
#define OP_000 0x066
#define OP_000 0x067
#define OP_000 0x068 // (front[n] & a) ? b : back[n]
#define OP_000 0x069
#define OP_000 0x06a
#define OP_000 0x06b
#define OP_000 0x06c // ((front[n] & a) ? back[n] : 0) + b
#define OP_000 0x06d
#define OP_000 0x06e
#define OP_000 0x06f

#define OP_000 0x070 // (i & a) ? back[i] - b : back[i] + b
#define OP_000 0x071
#define OP_000 0x072
#define OP_000 0x073
#define OP_000 0x074 // (i & a) ? back[i] + b : back[i] - b
#define OP_000 0x075
#define OP_000 0x076
#define OP_000 0x077
#define OP_000 0x078 // min(back[n], front[n])
#define OP_000 0x079
#define OP_000 0x07a
#define OP_000 0x07b
#define OP_000 0x07c // max(b, min(a, back[n]))
#define OP_000 0x07d
#define OP_000 0x07e
#define OP_000 0x07f

#define OP_000 0x080 // clamp(back[n], NEG2, POS2) * front[n]
#define OP_000 0x081
#define OP_000 0x082
#define OP_000 0x083
#define OP_000 0x084 // -(back[n] * front[n]) * 1[-2,2]​(front[n])
#define OP_000 0x085
#define OP_000 0x086
#define OP_000 0x087
#define OP_000 0x088 // (back[n] & a) == 0 ? b : 0
#define OP_000 0x089
#define OP_000 0x08a
#define OP_000 0x08b
#define OP_000 0x08c // (back[n] & a) != 0 ? b : 0
#define OP_000 0x08d
#define OP_000 0x08e
#define OP_000 0x08f

#define OP_000 0x090 // back[n] != 0 ? (b << back[n]) : b
#define OP_000 0x091
#define OP_000 0x092
#define OP_000 0x093
#define OP_000 0x094 // (back[n] <= 0) ? (b << (-back[n])) : 0
#define OP_000 0x095
#define OP_000 0x096
#define OP_000 0x097
#define OP_000 0x098 // (back[n] >= 0) ? (b << back[n]) & 0xFFFFFF : (b >> (-back[n]))
#define OP_000 0x099
#define OP_000 0x09a
#define OP_000 0x09b
#define OP_000 0x09c // (back[n] <= 0) ? (b << (-back[n])) : 0 // same as #define OP_000 0x094 ?
#define OP_000 0x09d
#define OP_000 0x09e
#define OP_000 0x09f

#define OP_000 0x0a0 // (front[n] >= 0) ? (back[n] << front[n]) : (back[n] >> -front[n])
#define OP_000 0x0a1 
#define OP_000 0x0a2
#define OP_000 0x0a3
#define OP_000 0x0a4 // (back[n] << b)
#define OP_000 0x0a5
#define OP_000 0x0a6
#define OP_000 0x0a7
#define OP_000 0x0a8 // (front[n] >= 0) ? (back[n] >> front[n]) : (back[n] << -front[n])
#define OP_000 0x0a9
#define OP_000 0x0aa
#define OP_000 0x0ab
#define OP_000 0x0ac // (back[n]) >> b
#define OP_000 0x0ad
#define OP_000 0x0ae
#define OP_000 0x0af

#define OP_000 0x0b0 // (front[n] >= 0) ? (back[n] << front[n]) : ((back[n] & 0xFFFFFF) >> -front[n])
#define OP_000 0x0b1
#define OP_000 0x0b2
#define OP_000 0x0b3
#define OP_000 0x0b4 // (back[n] & 0xFFFFFF) << b
#define OP_000 0x0b5
#define OP_000 0x0b6
#define OP_000 0x0b7
#define OP_000 0x0b8 // (front[n] >= 0) ? ((back[n] & 0xFFFFFF) >> front[n]) : (back[n] << -front[n])
#define OP_000 0x0b9
#define OP_000 0x0ba
#define OP_000 0x0bb
#define OP_000 0x0bc // (back[n] & 0xFFFFFF) >> b
#define OP_000 0x0bd
#define OP_000 0x0be
#define OP_000 0x0bf

#define OP_000 0x0c0 // (back[n] & front[n])
#define OP_000 0x0c1
#define OP_000 0x0c2
#define OP_000 0x0c3
#define OP_000 0x0c4 // (back[n] & b)
#define OP_000 0x0c5
#define OP_000 0x0c6
#define OP_000 0x0c7
#define OP_000 0x0c8 // ~(front[n] & back[n])
#define OP_000 0x0c9
#define OP_000 0x0ca
#define OP_000 0x0cb
#define OP_000 0x0cc // -(b & back[n]) - 1 // ~(b & back[n])
#define OP_000 0x0cd
#define OP_000 0x0ce
#define OP_000 0x0cf

#define OP_000 0x0d0 // (back[n] | front[n])
#define OP_000 0x0d1
#define OP_000 0x0d2
#define OP_000 0x0d3
#define OP_000 0x0d4 // (back[n] | b)
#define OP_000 0x0d5
#define OP_000 0x0d6
#define OP_000 0x0d7
#define OP_000 0x0d8 // ~(front[n] | back[n])
#define OP_000 0x0d9
#define OP_000 0x0da
#define OP_000 0x0db
#define OP_000 0x0dc // (-back[n] - 1) & ~b // (~back[n]) & ~b
#define OP_000 0x0dd
#define OP_000 0x0de
#define OP_000 0x0df

#define OP_000 0x0e0 // back[n] ^ front[n]
#define OP_000 0x0e1
#define OP_000 0x0e2
#define OP_000 0x0e3
#define OP_000 0x0e4 // (back[n] ^ b)
#define OP_000 0x0e5
#define OP_000 0x0e6
#define OP_000 0x0e7
#define OP_000 0x0e8 // ~(|front[n] - back[n]|)
#define OP_000 0x0e9
#define OP_000 0x0ea
#define OP_000 0x0eb
#define OP_000 0x0ec // (~back[n]) ^ b
#define OP_000 0x0ed
#define OP_000 0x0ee
#define OP_000 0x0ef

#define OP_000 0x0f0 // (back[n] == -1) ? 1 : 0
#define OP_000 0x0f1
#define OP_000 0x0f2
#define OP_000 0x0f3
#define OP_000 0x0f4 // (back[n] != 0) ? 1 : 0
#define OP_000 0x0f5
#define OP_000 0x0f6
#define OP_000 0x0f7
#define OP_000 0x0f8 // XOR(back[n].bit[0], ..., back[n].bit[23]) ? 1 : 0
#define OP_000 0x0f9
#define OP_000 0x0fa
#define OP_000 0x0fb
#define OP_000 0x0fc //  ~back[n] 
#define OP_000 0x0fd
#define OP_000 0x0fe
#define OP_000 0x0ff

#endif
