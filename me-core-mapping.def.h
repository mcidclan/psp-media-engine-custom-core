// Do not include this file - it serves as source data for the awk converter to ease function address mapping additions
// Note that the naming of the functions are not definitive
{
  {0x00000570, 0x00000000}, // int  _meCoreUnknown_00000570(int, int)             // _meCoreConfigureTransferMode(int, int) ?
  {0x00000948, 0x00000000}, // int  _meCoreUnknown_00000948()                     // _meCoreDspActivateAndSync() ?
  
  {0x000018fc, 0x00000000}, // void _meCoreBusClockEnableDMACPrimMux()
  {0x00001918, 0x00000000}, // int  _meCoreBusClockPreserveDMACPrimMux()
  {0x0000196c, 0x00000000}, // void _meCoreBusClockEnableKirk(void)               // _meCoreBusClockEnableDSP(void) ?
  {0x00001988, 0x00000000}, // void _meCoreBusClockDisableKirk(void)              // _meCoreBusClockPreserveDSP(void) ?
  {0x000019a4, 0x00001970}, // int  _meCoreBusClockEnable(u32)
  {0x000019f4, 0x000019c0}, // int  _meCoreBusClockPreserve(u32)                  // takes a mask which preserves the specified bits
  {0x00001a38, 0x00000000}, // int  _meCoreBusClockStorageEnableATAHDD()          // dsp related clock instead ?
  {0x00001a58, 0x00000000}, // void _meCoreBusClockStorageDisableATAHDD()         // dsp related clock instead ?
  {0x00001a78, 0x00000000}, // void _meCoreBusClockStorageEnableATA()             // dsp related clock instead ?
  {0x00001a98, 0x00000000}, // void _meCoreBusClockStorageDisableATA()            // dsp related clock instead ?
  {0x00001ab8, 0x00000000}, // int  _meCoreBusClockStorageEnableDisable(u32, u32) // mask, value
  {0x00001b14, 0x00000000}, // int  _meCoreBusClockMemoryStickSelect(u32)
  {0x00001bc4, 0x00000000}, // int  _meCoreSetBusFrequency(u32, u32);
  
  // dmac
  {0x000209d0, 0x00000000}, // void _meCoreDMACPrimMuxWaitStatus_0x800(void)
  {0x00020a08, 0x00000000}, // void _meCoreDMACPrimMuxSetState(u32)               // not sure
  {0x00020ca8, 0x00000000}, // void _meCoreDMACPrimMuxSetCtrl_0x002(void)         // init
  {0x00020cb8, 0x00000000}, // void _meCoreDMACPrimMuxSetCtrl_0x008(void)         // ?
  {0x00020cc8, 0x00000000}, // void _meCoreDMACPrimMuxSetCtrl_0x003(void)         // ?
  {0x00020cd8, 0x00000000}, // void _meCoreDMACPrimMuxSetCtrl_0x018(void)         // start
  {0x00020ce8, 0x00000000}, // void _meCoreDMACPrimarySetupBuffers(void)          // why ?
  {0x00021168, 0x00000000}, // void _meCoreDMACPrimMuxWaitStatus(u32)
  {0x000212cc, 0x00000000}, // u32* _meCoreMemcpy(u32*, u32*, u32)                // dst, src, size
  {0x00021338, 0x00000000}, // u32* _meCoreMemset(u32*, u32, u32)                 // dst, value, size
  {0x00021884, 0x00000000}, // int  _meCoreUnknown_00021884
  
  // thread related ?
  {0x00021910, 0x00000000}, // int  _meCorePriorityQueueInsert(u32,u32, int*)
  {0x000219b4, 0x00000000}, // u32  _meCorePriorityQueueDefaultInit(void)
  {0x00021a30, 0x00000000}, // int  _meCorePriorityQueueDispatch(void)
  
  {0x00021ad8, 0x00000000}, // int  _meCoreInterruptSetMask(u32)
  {0x00021ae4, 0x00000000}, // int  _meCoreInterruptClearMask(void)
  {0x00021af4, 0x00000000}, // int  _meCoreInterruptSetMask_2(u32)
  {0x00021b34, 0x00000000}, // int  _meCoreExceptionInitHandlers(void)
  {0x00021b7c, 0x00000000}, // int  _meCorePriorityQueueDefaultInsert(void)
  {0x00021bbc, 0x00000000}, // int  _meCoreEmitSoftwareInterrupt(void)
  
  // mutex
  {0x00021bd8, 0x0001324c}, // int _meCoreHwMutexTryLock(void)
  {0x00021c04, 0x00013278}, // int _meCoreHwMutexUnlock(void)
  {0x00021c20, 0x00013294}, // u32 _meCoreHwMutexAtomicRead(u32*)                 // target
  {0x00021c6c, 0x000132e0}, // u32 _meCoreHwMutexAtomicWrite(u32*, u32, u32)      // target, mask, value
  
  // cache
  {0x0008bc10, 0x0007b3bc}, // int _meCoreDcacheWritebackInvalidateAll(void)
  {0x0008bc38, 0x0007b3e8}, // int _meCoreDcacheWritebackInvalidateRange(u32, u32)
  {0x0008bd4c, 0x0008bd4c}, // int _meCoreDcacheWritebackRange(u32, u32)
  {0x0008bcbc, 0x0007b46c}, // int _meCoreDcacheInvalidateRange(u32, u32)
  
  // wip
  {0x000002e4, 0x00000000}, //      _meCoreUnknown_000002e4                       // meCoreSystemImageCopy AndReset ? // from what, to where ?
  {0x00000374, 0x00000000}, //      _meCoreWaitForScSyscalls                      // seems to handle syscall from sc
  {0x00000570, 0x00000000}, //      _meCoreUnknown_00000570                       // meCoreSetDSPFrequency ?
  {0x00000618, 0x00000000}, //      _meCoreUnknown_00000618    
  {0x00065a10, 0x00000000}, //      _meCoreCheck0xdec264                          // what is the purpose ?
  {0x00066804, 0x00000000}, //      _meCoreCopy164bytesFromOffset0x8308           // ??
  {0x00065908, 0x00000000}, //      _meCoreCheckAndCopy164bytesFromOffset0x8308   // ??
  {0x00000708, 0x00000000}, //      _meCoreUnknown_00000708
  {0x00021b34, 0x00000000}, // u64  _meCoreSetupHandlers(void)
  
  {0x0008be64, 0x00000000}, // int  _meCoreDecompressKL4E(byte *out, int size, char *in, u32 *cursor)
  {0x00021a30, 0x00000000}, // void _meCoreUpdate32PoolsMemory(void);
  {0x00021910, 0x00000000}, // u32  _meCoreRequest32PoolsBlockAllocation(u32 index, u32 priority, int* status)
  {0x000219b4, 0x00000000}, // void _meCoreInit32PoolsMemory(void)
  {0x000210b4, 0x00000000}, // void _meCoreDMACPrimaryProcessUnknown_000210b4(u32 memSrc, u32 bufSrc, u32 blockCount, u32 lastBlockIndex, u32 memDst, u32 bufDstLow, u32 bufDstHigh, u32 dstCount) // not sure
}
