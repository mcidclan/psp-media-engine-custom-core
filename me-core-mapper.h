#ifndef ME_CORE_MAPPER_H
#define ME_CORE_MAPPER_H

#include "hw-registers.h"

#define u32 unsigned          int

#define ME_CORE_IMG_TABLE     3
#define ME_CORE_T2_IMG_TABLE  2
#define ME_CORE_SD_IMG_TABLE  1
#define ME_CORE_BL_IMG_TABLE  0

#define ME_CORE_KERNEL_ADDR   0x88300000

#define sysCall(index)        ((void*)(meCoreBaseAddr + meCoreSystemTable[(index)]))

// me core functions
#define meCoreBusClockKirkEnable              ((void(*)(void))sysCall(0))
#define meCoreBusClockEnable                  ((int(*)(u32))sysCall(1))
#define meCoreBusClockFilterEnabled           ((int(*)(u32))sysCall(2))
#define meCoreBusClockATAHDDEnable            ((int(*)(void))sysCall(3))
#define meCoreEnableStorageATA                ((int(*)(void))sysCall(4))
#define meCoreEnableDisableStorageBusClock    ((int(*)(u32, u32))sysCall(5))
#define meCoreSelectMemoryStickBusClock       ((int(*)(u32))sysCall(6))
#define meCoreDMACMuxStatusWait_0x800         ((void(*)(void))sysCall(7))
#define meCoreDMACMuxCtrlSet_0x002            ((void(*)(void))sysCall(8)) // init
#define meCoreDMACMuxCtrlSet_0x008            ((void(*)(void))sysCall(9))
#define meCoreDMACMuxCtrlSet_0x003            ((void(*)(void))sysCall(10))
#define meCoreDMACMuxCtrlSet_0x018            ((void(*)(void))sysCall(11)) // start
#define meCoreDMACMuxStatusWait               ((void(*)(u32))sysCall(12))
#define meCoreMemcpy                          ((u32*(*)(u32*, u32*, u32))sysCall(13))
#define meCoreMemset                          ((u32*(*)(u32*, u32, u32))sysCall(14))
#define meCoreUnknown                         ((int(*)(void))sysCall(15))
#define meCorePriorityQueueInsert             ((int(*)(u32, u32, int*))sysCall(16))
#define meCorePriorityQueueDefaultInit        ((u32(*)(void))sysCall(17))
#define meCorePriorityQueueDispatch           ((int(*)(void))sysCall(18))
#define meCoreInterruptSetMask                ((int(*)(u32))sysCall(19))
#define meCoreInterruptClearMask              ((int(*)(void))sysCall(20))
#define meCoreInterruptSetMask2               ((int(*)(u32))sysCall(21))
#define meCoreExceptionInitHandlers           ((int(*)(void))sysCall(22))
#define meCorePriorityQueueDefaultInsert      ((int(*)(void))sysCall(23))
#define meCoreEmitSoftwareInterrupt           ((int(*)(void))sysCall(24))
#define meCoreHwMutexTryLock                  ((int(*)(void))sysCall(25))
#define meCoreHwMutexUnlock                   ((int(*)(void))sysCall(26))
#define meCoreDcacheWritebackInvalidateAll    ((void(*)(void))sysCall(27))


// me core tables

// img (fat)
const u32 meImgTable[] __attribute__((aligned(64))) = {
  0x0007b3bc, //      _meCoreDcacheWritebackInvalidateAll(void)
  0x00001970, //      _meCoreBusClockEnable(u32)
  0x000019c0, //      _meCoreBusClockFilterEnabled(u32)
  // todo:
  0,          //      _meCoreUnlockHwMutex(void)
  0,          //      _meCoreTryLockHwMutex(void)
  0,          //      _meCoreSelectMemoryStickBusClock(u32)
  0,          //      _meCoreEnableDisableStorageBusClock(u32, u32) // mask, value
};

// t2img (slim+)
const u32 t2ImgTable[] __attribute__((aligned(64))) = {
  // bus clock
  0x0000196c, // void _meCoreBusClockKirkEnable(void);
  0x000019a4, //      _meCoreBusClockEnable(u32)
  0x000019f4, //      _meCoreBusClockFilterEnabled(u32)
  0x00001a38, //      _meCoreBusClockATAHDDEnable();
  0x00001a78, // int  _meCoreBusClockEnableStorageATA()
  0x00001ab8, // int  _meCoreBusClockEnableDisableStorage(u32, u32) // mask, value
  0x00001b14, //      _meCoreBusClockSelectMemoryStick(u32)

  // dmac
  0x000209d0, // void _meCoreDMACMuxStatusWait_0x800(void)
  0x00020ca8, // void _meCoreDMACMuxCtrlSet_0x002(void) // init
  0x00020cb8, // void _meCoreDMACMuxCtrlSet_0x008(void) // ?
  0x00020cc8, // void _meCoreDMACMuxCtrlSet_0x003(void) // ?
  0x00020cd8, // void _meCoreDMACMuxCtrlSet_0x018(void) // start
  0x00021168, // void _meCoreDMACMuxStatusWait(u32)
  
  // mem
  0x000212cc, // u32* _meCoreMemcpy(u32*, u32*, u32)
  0x00021338, // u32* _meCoreMemset(u32*, u32, u32)

  0x00021884, //      _meCore ?
  
  // Queue, thread ?
  0x00021910, //      _meCorePriorityQueueInsert(u32,u32, int*)
  0x000219b4, // u32  _meCorePriorityQueueDefaultInit(void)
  0x00021a30, //      _meCorePriorityQueueDispatch(void)
  
  0x00021ad8, //      _meCoreInterruptSetMask(u32)
  0x00021ae4, //      _meCoreInterruptClearMask(void)
  0x00021af4, //      _meCoreInterruptSetMask(u32)
  0x00021b34, //      _meCoreExceptionInitHandlers(void)
  
  0x00021b7c, //      _meCorePriorityQueueDefaultInsert(void)
  0x00021bbc, //      _meCoreEmitSoftwareInterrupt(void)
  
  0x00021bd8, //      _meCoreHwMutexTryLock(void)
  0x00021c04, //      _meCoreHwMutexUnlock(void)

  0x0008bc10, //      _meCoreDcacheWritebackInvalidateAll(void)
};

// bl (?)
const u32 blImgTable[] __attribute__((aligned(64))) = {};

// sd (?)
const u32 sdImgTable[] __attribute__((aligned(64))) = {};

u32* meCoreSystemTable __attribute__((aligned(64))) = (u32*)t2ImgTable;
u32 meCoreBaseAddr __attribute__((aligned(64))) = ME_CORE_KERNEL_ADDR;

inline void meCoreSelectSystemTable(const u32 id) {
  switch(id) {
    case ME_CORE_T2_IMG_TABLE:
      meCoreSystemTable = (u32*)t2ImgTable;
      break;
    case ME_CORE_SD_IMG_TABLE:
      meCoreSystemTable = (u32*)sdImgTable;
      break;
    case ME_CORE_BL_IMG_TABLE:
      meCoreSystemTable = (u32*)blImgTable;
      break;
    default:
      meCoreSystemTable = (u32*)meImgTable;
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
