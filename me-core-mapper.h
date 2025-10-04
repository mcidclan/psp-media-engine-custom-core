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
#define meCoreUnknown_000002e4                    ((void  (*)(u32Me, u32Me))sysCall(0))
#define meCoreWaitForScSyscalls                   ((void  (*)(void))sysCall(1))
#define meCoreUnknown_00000570                    ((int   (*)(int, int))sysCall(2))
#define meCoreUnknown_00000618                    ((int   (*)(u32Me, u32Me, u32Me, u32Me))sysCall(3))
#define meCoreUnknown_00000708                    ((void  (*)(u32Me, u32Me, u32Me, u32Me, u32Me, u32Me, u32Me, u32Me))sysCall(4))
#define meCoreUnknown_00000948                    ((int   (*)(void))sysCall(5))
#define meCoreGenerate2DBlockFromQuad             ((u32Me (*)(u32Me*))sysCall(6))
#define meCoreGetDualBuffers                      ((int   (*)(u32Me, u32Me, u32Me*, u32Me*))sysCall(7))
#define meCoreCopy2DMemoryBlock                   ((void  (*)(u32Me*, void*, u32Me, u32Me, u32Me, u32Me, u32Me))sysCall(8))
#define meCoreInterleave2DBlockPixelData          ((void  (*)(u32Me*, void*, u32Me, u32Me, u32Me, u32Me, u32Me))sysCall(9))
#define meCoreCompose2DBlockFromQuad              ((u32Me (*)(u32Me*))sysCall(10))
#define meCoreBusClockAWEdramEnable               ((void  (*)(void))sysCall(11))
#define meCoreBusClockEnableDMACPrimMux           ((void  (*)(void))sysCall(12))
#define meCoreBusClockPreserveDMACPrimMux         ((int   (*)(void))sysCall(13))
#define meCoreBusClockEnableKirk                  ((void  (*)(void))sysCall(14))
#define meCoreBusClockDisableKirk                 ((void  (*)(void))sysCall(15))
#define meCoreBusClockEnable                      ((int   (*)(u32Me))sysCall(16))
#define meCoreBusClockPreserve                    ((int   (*)(u32Me))sysCall(17))
#define meCoreBusClockStorageEnableATAHDD         ((int   (*)(void))sysCall(18))
#define meCoreBusClockStorageDisableATAHDD        ((void  (*)(void))sysCall(19))
#define meCoreBusClockStorageEnableATA            ((void  (*)(void))sysCall(20))
#define meCoreBusClockStorageDisableATA           ((void  (*)(void))sysCall(21))
#define meCoreBusClockStorageEnableDisable        ((int   (*)(u32Me, u32Me))sysCall(22))
#define meCoreBusClockMemoryStickSelect           ((int   (*)(u32Me))sysCall(23))
#define meCoreSetCpuFrequency                     ((int   (*)(u32Me, u32Me))sysCall(24))
#define meCoreSetBusFrequency                     ((int   (*)(u32Me, u32Me))sysCall(25))
#define meCoreDMACPrimMuxWaitStatus_0x800         ((void  (*)(void))sysCall(26))
#define meCoreDMACPrimMuxSetState                 ((void  (*)(u32Me))sysCall(27))
#define meCoreDMACPrimMuxSetCtrl_0x002            ((void  (*)(void))sysCall(28))
#define meCoreDMACPrimMuxSetCtrl_0x008            ((void  (*)(void))sysCall(29))
#define meCoreDMACPrimMuxSetCtrl_0x003            ((void  (*)(void))sysCall(30))
#define meCoreDMACPrimMuxSetCtrl_0x018            ((void  (*)(void))sysCall(31))
#define meCoreDMACPrimaryTransferUnknown_00020ce8 ((void  (*)(void))sysCall(32))
#define meCoreDMACPrimaryProcessUnknown_000210b4  ((void  (*)(u32Me, u32Me, u32Me, u32Me, u32Me, u32Me, u32Me, u32Me))sysCall(33))
#define meCoreDMACPrimMuxWaitStatus               ((void  (*)(u32Me))sysCall(34))
#define meCoreMemcpy                              ((u32Me* (*)(u32Me*, u32Me*, u32Me))sysCall(35))
#define meCoreMemset                              ((u32Me* (*)(u32Me*, u32Me, u32Me))sysCall(36))
#define meCoreEDRAMAlloc                          ((void* (*)(u32Me))sysCall(37))
#define meCoreEDRAMFree                           ((u32Me (*)(void*))sysCall(38))
#define meCoreUnknown_00021884                    ((int   (*)(void))sysCall(39))
#define meCore32PoolsEnqueueEntry                 ((u32Me (*)(u32Me, u32Me, int*))sysCall(40))
#define meCore32PoolsInitMemory                   ((void  (*)(void))sysCall(41))
#define meCore32PoolsUpdateDispatchQueues         ((int   (*)(void))sysCall(42))
#define meCoreInterruptSetMask                    ((int   (*)(u32Me))sysCall(43))
#define meCoreInterruptClearMask                  ((int   (*)(void))sysCall(44))
#define meCoreInterruptSetMask_2                  ((int   (*)(u32Me))sysCall(45))
#define meCoreExceptionInitHandlers               ((u64   (*)(void))sysCall(46))
#define meCorePriorityQueueDefaultInsert          ((int   (*)(void))sysCall(47))
#define meCoreEmitSoftwareInterrupt               ((int   (*)(void))sysCall(48))
#define meCoreHwMutexTryLock                      ((int   (*)(void))sysCall(49))
#define meCoreHwMutexUnlock                       ((int   (*)(void))sysCall(50))
#define meCoreHwMutexAtomicRead                   ((u32Me (*)(u32Me*))sysCall(51))
#define meCoreHwMutexAtomicWrite                  ((u32Me (*)(u32Me*, u32Me, u32Me))sysCall(52))
#define meCoreProcessAudioMixer                   ((int   (*)(u32Me*, u32Me*, u32Me))sysCall(53))
#define meCoreProcessMultiChannelAudio            ((u32Me* (*)(u32Me*, u32Me*, int))sysCall(54))
#define meCoreConfigureReverbEffect               ((void  (*)(u32Me*, u32Me))sysCall(55))
#define meCoreProcessReverbEffectSamples          ((u32Me* (*)(u32Me**, u32Me*, u32Me*))sysCall(56))
#define meCoreIsIndexValidFor                     ((u32Me (*)(u32Me, u32Me*))sysCall(57))
#define meCoreGetIndexedValueFor                  ((u32Me (*)(int, u32Me*, void*))sysCall(58))
#define meCoreCheckAndCopy164bytesFromOffset0x8308 ((u32Me (*)(u32Me, u32Me))sysCall(59))
#define meCoreCheck0xdec264                       ((bool  (*)(u32Me*))sysCall(60))
#define meCoreCopy164bytesFromOffset0x8308        ((u32Me (*)(u32Me, u32Me*))sysCall(61))
#define meCoreAllocateSlotFrom                    ((void  (*)(u32Me**, u32Me))sysCall(62))
#define meCoreAllocateSlot                        ((u32Me* (*)(u32Me*))sysCall(63))
#define meCoreDcacheWritebackInvalidateAll        ((int   (*)(void))sysCall(64))
#define meCoreDcacheWritebackInvalidateRange      ((int   (*)(u32Me, u32Me))sysCall(65))
#define meCoreDcacheInvalidateRange               ((int   (*)(u32Me, u32Me))sysCall(66))
#define meCoreDcacheWritebackRange                ((int   (*)(u32Me, u32Me))sysCall(67))
#define meCoreDecompressKL4E                      ((int   (*)(byte*, int, char*, u32Me*))sysCall(68))
#define meCoreEDRAMAllocWords                     ((int   (*)(uint, uint))sysCall(69))

// t2img (slim+)
// img (fat), Todo.
// see me-core-mapping.def.h

// bl (?)
static const u32Me blImgTable[] __attribute__((aligned(64))) = {};
// sd (?)
static const u32Me sdImgTable[] __attribute__((aligned(64))) = {};

extern u32Me* meCoreSystemTable __attribute__((aligned(64)));

static inline void meCoreSelectSystemTable(const u32Me id) {
  switch(id) {
    case ME_CORE_BL_IMG_TABLE:
      meCoreSystemTable = (u32Me*)blImgTable;
      break;
    case ME_CORE_SD_IMG_TABLE:
      meCoreSystemTable = (u32Me*)sdImgTable;
      break;
    case ME_CORE_T2_IMG_TABLE:
      meCoreSystemTable = (u32Me*)t2ImgTable;
      break;
    default:
      meCoreSystemTable = (u32Me*)ogImgTable;
  }
}

// select table Id according the word at 0x88300018
static inline int meCoreGetTableIdFromWitnessWord() {
  const u32Me word = hw(ME_CORE_KERNEL_ADDR | 0x00000018);
  switch(word) {
    case 0x279ccba8: return ME_CORE_BL_IMG_TABLE;
    case 0x279cb300: return ME_CORE_SD_IMG_TABLE;
    case 0x279c637c: return ME_CORE_T2_IMG_TABLE;
    case 0x279c1d44: return ME_CORE_IMG_TABLE;
    default: return -1;
  }
}

#endif
