#ifndef ME_CORE_MAPPER_H
#define ME_CORE_MAPPER_H

#include "hw-registers.h"
#include "me-core-mapping.h"

#define ME_CORE_BL_IMG_TABLE  0
#define ME_CORE_SD_IMG_TABLE  1
#define ME_CORE_T2_IMG_TABLE  2
#define ME_CORE_IMG_TABLE     3

#define ME_CORE_KERNEL_ADDR   0x88300000

#define sysCall(index)        ((void*)(ME_CORE_KERNEL_ADDR + meCoreSystemTable[(index)]))

// me core functions
#define meCoreUnknown_00000570                    ((int   (*)(int, int))sysCall(0))
#define meCoreUnknown_00000948                    ((int   (*)(void))sysCall(1))
#define meCoreBusClockEnableDMACPrimMux           ((void  (*)(void))sysCall(2))
#define meCoreBusClockPreserveDMACPrimMux         ((int   (*)(void))sysCall(3))
#define meCoreBusClockEnableKirk                  ((void  (*)(void))sysCall(4))
#define meCoreBusClockDisableKirk                 ((void  (*)(void))sysCall(5))
#define meCoreBusClockEnable                      ((int   (*)(u32))sysCall(6))
#define meCoreBusClockPreserve                    ((int   (*)(u32))sysCall(7))
#define meCoreBusClockStorageEnableATAHDD         ((int   (*)(void))sysCall(8))
#define meCoreBusClockStorageDisableATAHDD        ((void  (*)(void))sysCall(9))
#define meCoreBusClockStorageEnableATA            ((void  (*)(void))sysCall(10))
#define meCoreBusClockStorageDisableATA           ((void  (*)(void))sysCall(11))
#define meCoreBusClockStorageEnableDisable        ((int   (*)(u32, u32))sysCall(12))
#define meCoreBusClockMemoryStickSelect           ((int   (*)(u32))sysCall(13))
#define meCoreSetBusFrequency                     ((int   (*)(u32, u32))sysCall(14))
#define meCoreDMACPrimMuxWaitStatus_0x800         ((void  (*)(void))sysCall(15))
#define meCoreDMACPrimMuxSetState                 ((void  (*)(u32))sysCall(16))
#define meCoreDMACPrimMuxSetCtrl_0x002            ((void  (*)(void))sysCall(17))
#define meCoreDMACPrimMuxSetCtrl_0x008            ((void  (*)(void))sysCall(18))
#define meCoreDMACPrimMuxSetCtrl_0x003            ((void  (*)(void))sysCall(19))
#define meCoreDMACPrimMuxSetCtrl_0x018            ((void  (*)(void))sysCall(20))
#define meCoreDMACPrimarySetupBuffers             ((void  (*)(void))sysCall(21))
#define meCoreDMACPrimMuxWaitStatus               ((void  (*)(u32))sysCall(22))
#define meCoreMemcpy                              ((u32*  (*)(u32*, u32*, u32))sysCall(23))
#define meCoreMemset                              ((u32*  (*)(u32*, u32, u32))sysCall(24))
#define meCoreUnknown_00021884                    ((int   (*)(void))sysCall(25))
#define meCorePriorityQueueInsert                 ((int   (*)(u32, u32, int*))sysCall(26))
#define meCorePriorityQueueDefaultInit            ((u32   (*)(void))sysCall(27))
#define meCorePriorityQueueDispatch               ((int   (*)(void))sysCall(28))
#define meCoreInterruptSetMask                    ((int   (*)(u32))sysCall(29))
#define meCoreInterruptClearMask                  ((int   (*)(void))sysCall(30))
#define meCoreInterruptSetMask_2                  ((int   (*)(u32))sysCall(31))
#define meCoreExceptionInitHandlers               ((int   (*)(void))sysCall(32))
#define meCorePriorityQueueDefaultInsert          ((int   (*)(void))sysCall(33))
#define meCoreEmitSoftwareInterrupt               ((int   (*)(void))sysCall(34))

#define meCoreHwMutexTryLock                      ((int   (*)(void))sysCall(35))
#define meCoreHwMutexUnlock                       ((int   (*)(void))sysCall(36))
#define meCoreHwMutexAtomicRead                   ((u32   (*)(u32*))sysCall(37))
#define meCoreHwMutexAtomicWrite                  ((u32   (*)(u32*, u32, u32))sysCall(38))

#define meCoreDcacheWritebackInvalidateAll        ((int   (*)(void))sysCall(39))
#define meCoreDcacheWritebackInvalidateRange      ((int   (*)(u32, u32))sysCall(40))
#define meCoreDcacheWritebackRange                ((int   (*)(u32, u32))sysCall(41))
#define meCoreDcacheInvalidateRange               ((int   (*)(u32, u32))sysCall(42))

// t2img (slim+)
// img (fat), Todo.
// see me-core-mapping.def.h

// bl (?)
static const u32 blImgTable[] __attribute__((aligned(64))) = {};
// sd (?)
static const u32 sdImgTable[] __attribute__((aligned(64))) = {};

extern u32* meCoreSystemTable __attribute__((aligned(64)));

static inline void meCoreSelectSystemTable(const u32 id) {
  switch(id) {
    case ME_CORE_BL_IMG_TABLE:
      meCoreSystemTable = (u32*)blImgTable;
      break;
    case ME_CORE_SD_IMG_TABLE:
      meCoreSystemTable = (u32*)sdImgTable;
      break;
    case ME_CORE_T2_IMG_TABLE:
      meCoreSystemTable = (u32*)t2ImgTable;
      break;
    default:
      meCoreSystemTable = (u32*)ogImgTable;
  }
}

// select table Id according the word at 0x88300018
static inline int meCoreGetTableIdFromWitnessWord() {
  const u32 word = hw(ME_CORE_KERNEL_ADDR | 0x00000018);
  switch(word) {
    case 0x279ccba8: return ME_CORE_BL_IMG_TABLE;
    case 0x279cb300: return ME_CORE_SD_IMG_TABLE;
    case 0x279c637c: return ME_CORE_T2_IMG_TABLE;
    case 0x279c1d44: return ME_CORE_IMG_TABLE;
    default: return -1;
  }
}

#endif
