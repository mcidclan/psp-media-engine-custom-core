#include "me-lib.h"
#include "me-core-mapper.h"

void vmeLibEnable() {
  
  meCoreBusClockEnableDMACPrimMux();
  meCoreBusClockEnableVMECtrl();
}

void vmeLibDisable() {

  meCoreBusClockDisableDMACPrimMux();
  meCoreBusClockDisableVMECtrl();
}

void vmeLibConfigTransfer(const int status) {
  
  hw(0x440ff000) |= status | 0x80; // 0;   // set default status
  hw(0x440ff004) |= 0x10;                  // set default config
  meLibSync();
}

void vmeLibWipe() {
  
  vmeLibConfigTransfer(0);

  vmeLibStart();
  vme_icn(INPUT, 0x3210);
  vme_icn(FLOW, 0x3210);
  vme_icn(ARCH, 0x3210);
  vme_set(ENABLE, FU_1, 0xf0000000);
  vme_pe0(agu_top(MODE), 0x84000000);
  vme_pe0(agu_top(COUNT), 0x00010000, 2048 - 1);
  vme_pe0(vme_fu(PRIMARY), 0x00004000);
  vme_pe0(vme_fu(SECONDARY), 0x00004000);
  vme_pe1(vme_fu(PRIMARY), 0x00004000);
  vme_pe1(vme_fu(SECONDARY), 0x00004000);
  vme_pe2(vme_fu(PRIMARY), 0x00004000);
  vme_pe2(vme_fu(SECONDARY), 0x00004000);
  vme_pe3(vme_fu(PRIMARY), 0x00004000);
  vme_pe3(vme_fu(SECONDARY), 0x00004000);
  vmeLibFinish();
  
  _vmeLibStart();
  meCoreMemset((void*)VME_DATAPATH_BASE, 0, 0x01a8); // 0x400
  _vmeLibFinish();

  vmeLibClearLocalBuffer(0, 0x2000);
  vmeLibClearLocalBuffer(0x8000, 0x2000);
  
  // meCoreMemset((void*)VME_TOP_BUFFERS, 0, 0x8000);
  // meCoreMemset((void*)VME_BASE_BUFFERS, 0, 0x8000);
  
  vmeLibSetInnerAGU1(0, 0, 0);
  vmeLibSetInnerAGU2(0, 0, 0);
  hw(0x440ff01c) = 0;
  hw(0x440ff02c) = 0;
  hw(0x440ff03c) = 0;
  meLibSync();
}

void vmeLibSendCustomContext(void* context) {
  
  vmeLibConfigTransfer(0x1c);

  hw(0x440ff010) = 0x40000000 | (u32)context; // context source address
  hw(0x440ff008) = 0x1c; // 0x1d;             // minimal control value for context transfer
  meLibSync();
  
  meCoreDMACPrimWaitTransferFinish();
  //meCoreDMACPrimWaitVMEFinish();
}

void vmeLibClearLocalBuffer(const int dst, const int count) {
  
  vmeLibConfigTransfer(0);

  hw(0x440ff014) = count - 1;
  hw(0x440ff018) = dst;
  hw(0x440ff008) = 0x21;
  meCoreDMACPrimWaitTransferFinish();
}

void vmeLibTrigger() {
  
  meCoreDMACPrimMuxSetCtrl_0x018();
}

void vmeLibSetFlow(const int mode) {
  
  hw(0x440ff008) = mode;
}

void _vmeLibStart() {
  
  vmeLibSetFlow(VME_DEFAULT);
  meLibSync();
}

void _vmeLibFinish() {

  vmeLibTrigger();
  meCoreDMACPrimWaitVMEFinish();
}

void vmeLibMemoryToRingBuffer(void* const src, const u32 dstOff, const u32 count) {
  
  meCoreDMACPrimMemoryToRingBuffer(src, dstOff, count);
  meCoreDMACPrimWaitTransferFinish();
}

void vmeLibRingBufferToMemory(const u32 srcOff, void* const dst, const u32 count) {
  
  meCoreDMACPrimRingBufferToMemory(dst, srcOff, count);
  meCoreDMACPrimWaitTransferFinish();
}

void vmeLibMemTo16(const u32 src, const int offset, const int count, const int wait) {

  hw(0x440ff010) = (src & 0x1fffffff) | 0x40000000;
  hw(0x440ff014) = count - 1;
  hw(0x440ff018) = offset;
  hw(0x440ff008) = 0x52;
  
  if (wait) {
    meCoreDMACPrimWaitTransferFinish();
    return;
  }
  meLibSync();
}

void vmeLibMemFrom16(const u32 dst, const int offset, const int count, const int wait) {

  hw(0x440ff010) = (dst & 0x1fffffff) | 0x40000000;
  hw(0x440ff014) = count - 1;
  hw(0x440ff018) = offset;
  hw(0x440ff008) = 0x5a;
  
  if (wait) {
    meCoreDMACPrimWaitTransferFinish();
    return;
  }
  meLibSync();
}

void vmeLibSetInnerAGU1(const int offset, const int count, const int stride) {

  hw(0x440ff020) = offset;
  hw(0x440ff024) = count;
  hw(0x440ff028) = stride;
}

void vmeLibSetInnerAGU2(const int offset, const int count, const int stride) {

  hw(0x440ff030) = offset;
  hw(0x440ff034) = count;
  hw(0x440ff038) = stride;
}

