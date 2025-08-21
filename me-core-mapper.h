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


enum {
  _meCoreDcacheWritebackInvalidateAll,
  _meCore_00001,
  _meCore_00002,
  count
};

// core tables
const u32 meImgTable[count] __attribute__((aligned(64))) = {
  (ME_CORE_KERNEL_ADDR | 0),
  (ME_CORE_KERNEL_ADDR | 0),
  (ME_CORE_KERNEL_ADDR | 0)
  //...
};

const u32 blImgTable[count] __attribute__((aligned(64))) = {
  (ME_CORE_KERNEL_ADDR | 0),
  (ME_CORE_KERNEL_ADDR | 0),
  (ME_CORE_KERNEL_ADDR | 0)
  //...
};

const u32 sdImgTable[count] __attribute__((aligned(64))) = {
  (ME_CORE_KERNEL_ADDR | 0),
  (ME_CORE_KERNEL_ADDR | 0),
  (ME_CORE_KERNEL_ADDR | 0)
  //...
};

// t2img (slim+)
const u32 t2ImgTable[count] __attribute__((aligned(64))) = {
  (ME_CORE_KERNEL_ADDR | 0x0008bc10),
  (ME_CORE_KERNEL_ADDR | 0),
  (ME_CORE_KERNEL_ADDR | 0x00001a78)
  //...
};

u32* systemTable = (u32*)t2ImgTable;

inline void meCoreSelectSystemTable(const u32 id) {
  switch(id) {
    case 0:
      systemTable = (u32*)t2ImgTable;
      break;
    case 1:
      systemTable = (u32*)sdImgTable;
      break;
    case 2:
      systemTable = (u32*)blImgTable;
      break;
    default:
      systemTable = (u32*)meImgTable;
  }
}

#endif
