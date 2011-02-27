#include "gm_luaerror.h"

SETUP_SIMPLE_DETOUR(DrawError_Detour, CDetour::DrawError_T, CDetour::DrawError_H);

void CDetour::DrawError_H(const char *err, ...)
{
	char szBuf[256];

	va_list arg_ptr;

	va_start(arg_ptr, err);
	_vsntprintf(szBuf, sizeof(szBuf)-1, (char *)err, arg_ptr);
	va_end(arg_ptr);

	ILuaObject *hookT = gLua->GetGlobal("hook");
		ILuaObject *hookM = hookT->GetMember("Call");
			hookM->Push();

			gLua->Push("LuaError");
			gLua->PushNil();
			gLua->Push(szBuf);

			gLua->Call(3, 1);
		hookM->UnReference();
	hookT->UnReference();

	ILuaObject *ret = gLua->GetReturn(0);
	bool isNil = ret->isNil();
	ret->UnReference();

	if (isNil)
		(this->*DrawError_T)(szBuf);
}

int Open(lua_State *L)
{
	gLua = Lua();

	CSimpleScan lua_sharedInterface("lua_shared.dll");

	if (lua_sharedInterface.FindFunction("\x8B\x4C\x24\x08\x81\xEC\x00\x02\x00\x00\x8D\x84\x24\x0C\x02\x00\x00\x50\x51\x8D\x54\x24\x08\x68\x00\x02\x00\x00\x52\xE8\xCE\xFB\x01\x00\x8B\x84\x24\x14\x02\x00\x00\x8B\x88\x88\x10\x00\x00\x8B\x11\x8B\x52\x08\x83\xC4\x10\x8D\x04\x24", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxx", (void **)&CDetour::DrawError_T))
		DrawError_Detour.Attach();

	//8B 4C 24 08 81 EC 00 02 00 00 8D 84 24 0C 02 00 00 50 51 8D 54 24 08 68 00 02 00 00 52 E8 CE FB
	//01 00 8B 84 24 14 02 00 00 8B 88 88 10 00 00 8B 11 8B 52 08 83 C4 10 8D 04 24

	//8B 4C 24 08 81 EC 00 02 00 00 8D 84 24 0C 02 00 00 50 51 8D 54 24 08 68 00 02 00 00 52 E8 ?? ??
	//?? ?? 8B 84 24 14 02 00 00 8B 88 88 10 00 00 8B 11 8B 52 08 83 C4 10 8D 04 24

	return 0;
}

int Close(lua_State *L)
{
	DrawError_Detour.Detach();

	return 0;
}