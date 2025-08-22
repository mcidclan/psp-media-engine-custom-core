#ifndef ME_CORE_MAPPER_H
#define ME_CORE_MAPPER_H

#include "hw-registers.h"

#define u32                                 unsigned int

#define ME_CORE_T2_IMG_TABLE                0
#define ME_CORE_SD_IMG_TABLE                1
#define ME_CORE_BL_IMG_TABLE                2
#define ME_CORE_IMG_TABLE                   3

#define ME_CORE_BASE_ADDR                   0x88300000
#define ME_CORE_KERNEL_ADDR                 0x88300000

#define sysCall(index)                      ((void*)systemTable[(index)])

// me core functions
#define meCoreDcacheWritebackInvalidateAll    ((void(*)(void))sysCall(_meCoreDcacheWritebackInvalidateAll))
// ..
#define meCoreBusClockEnable                  ((int(*)(u32))sysCall(_meCoreBusClockEnable))
#define meCoreBusClockFilterEnabled           ((int(*)(u32))sysCall(_meCoreBusClockFilterEnabled))
#define meCoreUnlockHwMutex                   ((int(*)(void))sysCall(_meCoreUnlockHwMutex))
#define meCoresceKernelTryLockHwMutex         ((int(*)(void))sysCall(_meCoreTryLockHwMutex))
#define meCoreSelectMemoryStickBusClock       ((int(*)(u32))sysCall(_meCoreSelectMemoryStickBusClock))
#define meCoreEnableDisableStorageBusClock    ((int(*)(u32, u32))sysCall(_meCoreEnableDisableStorageBusClock))


enum {
  _meCoreDcacheWritebackInvalidateAll,
  _meCoreBusClockEnable,
  _meCoreBusClockFilterEnabled,
  _meCoreUnlockHwMutex,
  _meCoreTryLockHwMutex,
  _meCoreSelectMemoryStickBusClock,
  _meCoreEnableDisableStorageBusClock,
  count
};

// core tables

// img
const u32 meImgTable[count] __attribute__((aligned(64))) = {
  (ME_CORE_KERNEL_ADDR | 0x0007b3bc), // _meCoreDcacheWritebackInvalidateAll(void)
  (ME_CORE_KERNEL_ADDR | 0x00001970), // _meCoreBusClockEnable(u32)
  (ME_CORE_KERNEL_ADDR | 0x000019c0), // _meCoreBusClockFilterEnabled(u32)
};

// bl
const u32 blImgTable[count] __attribute__((aligned(64))) = {
  (ME_CORE_KERNEL_ADDR | 0x00031398), // _meCoreDcacheWritebackInvalidateAll(void)
  (ME_CORE_KERNEL_ADDR | 0x00001520), // _meCoreBusClockEnable(u32)
  (ME_CORE_KERNEL_ADDR | 0x00001570), // _meCoreBusClockFilterEnabled(u32)
};

// sd
const u32 sdImgTable[count] __attribute__((aligned(64))) = {
  (ME_CORE_KERNEL_ADDR | 0x0002ddd0), // _meCoreDcacheWritebackInvalidateAll(void)
  (ME_CORE_KERNEL_ADDR | 0x00001660), // _meCoreBusClockEnable(u32)
  (ME_CORE_KERNEL_ADDR | 0x000016b0), // _meCoreBusClockFilterEnabled(u32)
};

// t2img (slim and slim+ ?)
const u32 t2ImgTable[count] __attribute__((aligned(64))) = {
  (ME_CORE_KERNEL_ADDR | 0x0008bc10), // _meCoreDcacheWritebackInvalidateAll(void)
  (ME_CORE_KERNEL_ADDR | 0x000019a4), // _meCoreBusClockEnable(u32)
  (ME_CORE_KERNEL_ADDR | 0x000019f4), // _meCoreBusClockFilterEnabled(u32)
  (ME_CORE_KERNEL_ADDR | 0x00021c04), // _meCoreUnlockHwMutex(void)
  (ME_CORE_KERNEL_ADDR | 0x00021bd8), // _meCoreTryLockHwMutex(void)
  (ME_CORE_KERNEL_ADDR | 0x00001b14), // _meCoreSelectMemoryStickBusClock(u32)
  (ME_CORE_KERNEL_ADDR | 0x00001ab8), // _meCoreEnableDisableStorageBusClock(u32, u32) // mask, value
};

u32* systemTable = (u32*)t2ImgTable;

inline void meCoreSelectSystemTable(const u32 id) {
  switch(id) {
    case ME_CORE_T2_IMG_TABLE:
      systemTable = (u32*)t2ImgTable;
      break;
    case ME_CORE_SD_IMG_TABLE:
      systemTable = (u32*)sdImgTable;
      break;
    case ME_CORE_BL_IMG_TABLE:
      systemTable = (u32*)blImgTable;
      break;
    default:
      systemTable = (u32*)meImgTable;
  }
}

// select table Id according the word at 0x88300018
inline int meCoreGetTableIdFromWitnessWord() {
  const u32 word = hw(ME_CORE_KERNEL_ADDR | 0x00000018);
  switch(word) {
    case 0x279c637c: return ME_CORE_T2_IMG_TABLE;
    case 0x279cb300: return ME_CORE_SD_IMG_TABLE;
    case 0x279ccba8: return ME_CORE_BL_IMG_TABLE;
    case 0x279c1d44: return ME_CORE_IMG_TABLE;
    default: return -1;
  }
}

#endif
