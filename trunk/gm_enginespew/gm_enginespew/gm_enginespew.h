#include "gminterface/include/GMLuaModule.h"
#include <tier0/dbg.h>
//#include <color.h>

GMOD_MODULE(Open, Close);

ILuaInterface *gLua = NULL;
SpewOutputFunc_t oldSpewFunction = NULL;