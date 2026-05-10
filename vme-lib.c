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

void vmeLibConfigTransfer() {
  
  hw(0x440ff000) = 0;                           // minimal default status
  hw(0x440ff004) = 0x10;                        // minimal default config
}

void vmeLibWipe() {
  
  vmeLibConfigTransfer();

  _vmeLibStart();
  meCoreMemset((void*)VME_DATAPATH_BASE, 0, 0x01a8); // 0x400
  _vmeLibFinish();

  //vmeLibClearLocalBuffer(0, 0x2000);
  //vmeLibClearLocalBuffer(0x8000, 0x2000);

  meCoreMemset((void*)VME_TOP_BUFFERS, 0, 0x8000);
  meCoreMemset((void*)VME_BASE_BUFFERS, 0, 0x8000);
}

void vmeLibSendCustomBitstream(void* bitstream) {
  
  vmeLibConfigTransfer();

  hw(0x440ff010) = 0x40000000 | (u32)bitstream; // bitstream source address
  hw(0x440ff008) = 0x1c; // 0x1d;               // minimal control value for bitstream transfer
  meLibSync();
  
  meCoreDMACPrimWaitTransferFinish();
  meCoreDMACPrimWaitVMEFinish();
}

void vmeLibClearLocalBuffer(const int dst, const int count) {
  
  vmeLibConfigTransfer();

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
