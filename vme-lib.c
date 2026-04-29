#include "me-lib.h"
#include "me-core-mapper.h"

void vmeLibInit() {
  
  meLibSetMinimalVmeConfig();
  meCoreExceptionInitHandlers();
  meCoreBusClockEnableDMACPrimMux();
  meCoreBusClockEnableVMECtrl();
  
  hw(0x440ff000) = 0;      // default status
  hw(0x440ff004) = 0x10;   // minimal default config
  meLibSync();
}

void vmeLibSendCustomBitstream(void* bitstream) {
  
  hw(0x440ff010) = 0x40000000 | (u32)bitstream; // bitstream source address
  hw(0x440ff008) = 0x1c;                        // minimal control value for bitstream transfer
  meLibSync();
  
  meCoreDMACPrimWaitTransferFinish();
  meCoreDMACPrimWaitVMEFinish();
}

void vmeLibRefreshProcess() {

  meCoreDMACPrimMuxSetCtrl_0x018();
  meCoreDMACPrimWaitVMEFinish();
}
