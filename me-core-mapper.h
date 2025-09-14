#ifndef ME_CORE_MAPPER_H
#define ME_CORE_MAPPER_H

#include "hw-registers.h"
#include "me-core-mapping.h"

#define ME_CORE_BL_IMG_TABLE  0
#define ME_CORE_SD_IMG_TABLE  1
#define ME_CORE_T2_IMG_TABLE  2
#define ME_CORE_IMG_TABLE     3

#define ME_CORE_KERNEL_ADDR   0x88300000
#define ME_CORE_BASE_ADDR     ME_CORE_KERNEL_ADDR

#define sysCall(index)        ((void*)(ME_CORE_BASE_ADDR + meCoreSystemTable[(index)]))

// me core functions
#define meCoreUnknown_000002e4                    ((void  (*)(u32, u32))sysCall(0))
#define meCoreWaitForScSyscalls                   ((void  (*)(void))sysCall(1))
#define meCoreUnknown_00000570                    ((int   (*)(int, int))sysCall(2))
#define meCoreUnknown_00000618                    ((int   (*)(u32, u32, u32, u32))sysCall(3))
#define meCoreUnknown_00000708                    ((void  (*)(u32, u32, u32, u32, u32, u32, u32, u32))sysCall(4))
#define meCoreUnknown_00000948                    ((int   (*)(void))sysCall(5))
#define meCoreBusClockEnableDMACPrimMux           ((void  (*)(void))sysCall(6))
#define meCoreBusClockPreserveDMACPrimMux         ((int   (*)(void))sysCall(7))
#define meCoreBusClockEnableKirk                  ((void  (*)(void))sysCall(8))
#define meCoreBusClockDisableKirk                 ((void  (*)(void))sysCall(9))
#define meCoreBusClockEnable                      ((int   (*)(u32))sysCall(10))
#define meCoreBusClockPreserve                    ((int   (*)(u32))sysCall(11))
#define meCoreBusClockStorageEnableATAHDD         ((int   (*)(void))sysCall(12))
#define meCoreBusClockStorageDisableATAHDD        ((void  (*)(void))sysCall(13))
#define meCoreBusClockStorageEnableATA            ((void  (*)(void))sysCall(14))
#define meCoreBusClockStorageDisableATA           ((void  (*)(void))sysCall(15))
#define meCoreBusClockStorageEnableDisable        ((int   (*)(u32, u32))sysCall(16))
#define meCoreBusClockMemoryStickSelect           ((int   (*)(u32))sysCall(17))
#define meCoreSetBusFrequency                     ((int   (*)(u32, u32))sysCall(18))
#define meCoreDMACPrimMuxWaitStatus_0x800         ((void  (*)(void))sysCall(19))
#define meCoreDMACPrimMuxSetState                 ((void  (*)(u32))sysCall(20))
#define meCoreDMACPrimMuxSetCtrl_0x002            ((void  (*)(void))sysCall(21))
#define meCoreDMACPrimMuxSetCtrl_0x008            ((void  (*)(void))sysCall(22))
#define meCoreDMACPrimMuxSetCtrl_0x003            ((void  (*)(void))sysCall(23))
#define meCoreDMACPrimMuxSetCtrl_0x018            ((void  (*)(void))sysCall(24))
#define meCoreDMACPrimarySetupBuffers             ((void  (*)(void))sysCall(25))
#define meCoreDMACPrimaryProcessUnknown_000210b4  ((void  (*)(u32, u32, u32, u32, u32, u32, u32, u32))sysCall(26))
#define meCoreDMACPrimMuxWaitStatus               ((void  (*)(u32))sysCall(27))
#define meCoreMemcpy                              ((u32*  (*)(u32*, u32*, u32))sysCall(28))
#define meCoreMemset                              ((u32*  (*)(u32*, u32, u32))sysCall(29))
#define meCoreUnknown_00021884                    ((int   (*)(void))sysCall(30))
#define meCorePriorityQueueInsert                 ((int   (*)(u32, u32, int*))sysCall(31))
#define meCoreRequest32PoolsBlockAllocation       ((u32   (*)(u32, u32, int*))sysCall(32))
#define meCorePriorityQueueDefaultInit            ((u32   (*)(void))sysCall(33))
#define meCoreInit32PoolsMemory                   ((void  (*)(void))sysCall(34))
#define meCorePriorityQueueDispatch               ((int   (*)(void))sysCall(35))
#define meCoreUpdate32PoolsMemory                 ((void  (*)(void))sysCall(36))
#define meCoreInterruptSetMask                    ((int   (*)(u32))sysCall(37))
#define meCoreInterruptClearMask                  ((int   (*)(void))sysCall(38))
#define meCoreInterruptSetMask_2                  ((int   (*)(u32))sysCall(39))
#define meCoreExceptionInitHandlers               ((int   (*)(void))sysCall(40))
#define meCoreSetupHandlers                       ((u64   (*)(void))sysCall(41))
#define meCorePriorityQueueDefaultInsert          ((int   (*)(void))sysCall(42))
#define meCoreEmitSoftwareInterrupt               ((int   (*)(void))sysCall(43))
#define meCoreHwMutexTryLock                      ((int   (*)(void))sysCall(44))
#define meCoreHwMutexUnlock                       ((int   (*)(void))sysCall(45))
#define meCoreHwMutexAtomicRead                   ((u32   (*)(u32*))sysCall(46))
#define meCoreHwMutexAtomicWrite                  ((u32   (*)(u32*, u32, u32))sysCall(47))
#define meCoreCheckAndCopy164bytesFromOffset0x8308 ((u32  (*)(u32, u32))sysCall(48))
#define meCoreCheck0xdec264                       ((bool  (*)(u32*))sysCall(49))
#define meCoreCopy164bytesFromOffset0x8308        ((u32   (*)(u32, u32*))sysCall(50))
#define meCoreDcacheWritebackInvalidateAll        ((int   (*)(void))sysCall(51))
#define meCoreDcacheWritebackInvalidateRange      ((int   (*)(u32, u32))sysCall(52))
#define meCoreDcacheInvalidateRange               ((int   (*)(u32, u32))sysCall(53))
#define meCoreDcacheWritebackRange                ((int   (*)(u32, u32))sysCall(54))
#define meCoreDecompressKL4E                      ((int   (*)(byte*, int, char*, u32*))sysCall(55))

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
