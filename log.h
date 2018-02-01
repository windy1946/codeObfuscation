#include "llvm/Support/raw_ostream.h"

#define LOGI(fmt) llvm::errs()<<"I::"<<fmt<<"\n";
#define LOGE(fmt) llvm::errs()<<"E::"<<fmt<<"\n";

#ifdef NDEBUG
#define LOGD(fmt)
#define LOGDLN(fmt)
#else
#define LOGD(fmt) llvm::errs()<<"D::"<<fmt;
#define LOGDLN(fmt) llvm::errs()<<"D::"<<fmt<<"\n";
#endif
