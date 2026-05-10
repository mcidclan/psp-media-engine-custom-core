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

void vmeLibWipe() {
  
  //meLibSetMinimalVmeConfig();
  
  _vmeLibStart();
  meCoreMemset((void*)VME_DATAPATH_BASE, 0, 0x400*4/*0x01a8*/);
  _vmeLibFinish();

  meCoreMemset((void*)VME_TOP_BUFFERS, 0, 0x8000);
  meCoreMemset((void*)VME_BASE_BUFFERS, 0, 0x8000);
}

void vmeLibSendCustomBitstream(void* bitstream) {
  
  hw(0x440ff000) = 0;                           // minimal default status
  hw(0x440ff004) = 0x10;                        // minimal default config
  hw(0x440ff010) = 0x40000000 | (u32)bitstream; // bitstream source address
  hw(0x440ff008) = 0x1c;                        // minimal control value for bitstream transfer
  meLibSync();
  
  meCoreDMACPrimWaitTransferFinish();
  meCoreDMACPrimWaitVMEFinish();
}

void vmeLibClearLocalBuffer(const int dst, const int count) {
  
  hw(0x440ff014) = count - 1;
  hw(0x440ff018) = dst;
  hw(0x440ff008) = 0x21;
  meCoreDMACPrimWaitTransferFinish();
}

void _vmeLibStart() {
    
  meCoreDMACPrimMuxSetCtrl_0x003();
}

void _vmeLibFinish() {

  meCoreDMACPrimMuxSetCtrl_0x018();
  meCoreDMACPrimWaitVMEFinish();
}
