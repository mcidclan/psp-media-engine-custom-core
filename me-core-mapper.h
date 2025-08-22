#ifndef ME_CORE_MAPPER_H
#define ME_CORE_MAPPER_H

#define u32                                 unsigned int

#define ME_CORE_T2_IMG_TABLE                0
#define ME_CORE_SD_IMG_TABLE                1
#define ME_CORE_BL_IMG_TABLE                2
#define ME_CORE_IMG_TABLE                   3

#define ME_CORE_KERNEL_ADDR                 0x88300000

#define sysCall(index)                      ((void*)systemTable[(index)])

// me core functions
#define meCoreDcacheWritebackInvalidateAll  ((void(*)(void))sysCall(_meCoreDcacheWritebackInvalidateAll))
#define meCoreBusClockEnable                ((int(*)(u32))sysCall(_meCoreBusClockEnable))
#define meCoreBusClockFilterEnabled         ((int(*)(u32))sysCall(_meCoreBusClockFilterEnabled))


enum {
  _meCoreDcacheWritebackInvalidateAll,
  _meCoreBusClockEnable,
  _meCoreBusClockFilterEnabled,
  count
};

// core tables

// img (fat)
const u32 meImgTable[count] __attribute__((aligned(64))) = {
  (ME_CORE_KERNEL_ADDR | 0x0007b3bc), // _meCoreDcacheWritebackInvalidateAll
  (ME_CORE_KERNEL_ADDR | 0x00001970), // _meCoreBusClockEnable
  (ME_CORE_KERNEL_ADDR | 0x000019c0), // _meCoreBusClockFilterEnabled
};

/*
const u32 blImgTable[count] __attribute__((aligned(64))) = {
};
const u32 sdImgTable[count] __attribute__((aligned(64))) = {
};
*/

// t2img (slim+)
const u32 t2ImgTable[count] __attribute__((aligned(64))) = {
  (ME_CORE_KERNEL_ADDR | 0x0008bc10), // _meCoreDcacheWritebackInvalidateAll
  (ME_CORE_KERNEL_ADDR | 0x000019a4), // _meCoreBusClockEnable
  (ME_CORE_KERNEL_ADDR | 0x000019f4), // _meCoreBusClockFilterEnabled
};

u32* systemTable = (u32*)t2ImgTable;

inline void meCoreSelectSystemTable(const u32 id) {
  switch(id) {
    case ME_CORE_T2_IMG_TABLE:
      systemTable = (u32*)t2ImgTable;
      break;
    case ME_CORE_SD_IMG_TABLE:
      systemTable = NULL; //(u32*)sdImgTable;
      break;
    case ME_CORE_BL_IMG_TABLE:
      systemTable = NULL; //(u32*)blImgTable;
      break;
    default:
      systemTable = (u32*)meImgTable;
  }
}

#endif
