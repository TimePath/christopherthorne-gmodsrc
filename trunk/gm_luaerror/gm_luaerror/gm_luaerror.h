#ifndef GM_LUAERROR_H
#define GM_LUAERROR_H

#include "gminterface/include/GMLuaModule.h"
#include "simplescan/include/csimplescan.h"
#include "simpledetour/include/csimpledetour.h"

#include <interface.h>
#include <windows.h>

#include "cdetour.h"

GMOD_MODULE(Open, Close);

ILuaInterface *gLua = NULL;

CSigScan DrawError_Sig;

#endif //GM_LUAERROR_H