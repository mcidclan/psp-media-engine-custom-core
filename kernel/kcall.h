#ifndef KCALL_LIB_H
#define KCALL_LIB_H
#ifdef __cplusplus
extern "C" {
#endif
  typedef int (*FCall)(void);
  int kcall(FCall const func);
  int kinit(const void* const handler);
#ifdef __cplusplus
}
#endif
#endif
