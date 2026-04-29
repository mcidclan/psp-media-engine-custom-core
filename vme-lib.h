#ifndef ME_VME_LIB_H
#define ME_VME_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

void meLibVMEInit();
void meLibSendCustomVMEBitstream(void* bitstream);

#ifdef __cplusplus
}
#endif

#endif

