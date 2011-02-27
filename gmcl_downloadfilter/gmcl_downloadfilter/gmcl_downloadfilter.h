#ifndef GMCL_DOWNLOADFILTER_H
#define GMCL_DOWNLOADFILTER_H

#include "gminterface/include/GMLuaModule.h"
#include "sigscan/include/sigscan.h"
#include "detours/include/detours.h"

#include <eiface.h>

GMOD_MODULE(Open, Close);

ILuaInterface *gLua = NULL;
IVEngineServer *engine = NULL;
CSigScan ShouldDownload_Sig;

typedef bool (*ShouldDownload_t)(const char *);
bool (*ShouldDownload_T)(const char *) = NULL;

#endif //GMCL_DOWNLOADFILTER_H