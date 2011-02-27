#include "gm_enginespew.h"

SpewRetval_t SpewFunction(SpewType_t spewType, const char *msg)
{
	ILuaObject *hookT = gLua->GetGlobal("hook");
		ILuaObject *hookM = hookT->GetMember("Call");
			hookM->Push();

			gLua->Push("EngineSpew");
			gLua->PushNil();
			gLua->PushDouble(spewType);
			gLua->Push(msg);
			gLua->Push(GetSpewOutputGroup());
			gLua->PushDouble(GetSpewOutputLevel());

			/*const Color &color = GetSpewOutputColor(); //Always white?

			ILuaObject *col = gLua->GetNewTable();
				col->SetMember("r", (float)color.r());
				col->SetMember("g", (float)color.g());
				col->SetMember("b", (float)color.b());
				col->SetMember("a", (float)color.a());

				col->Push();
			col->UnReference();

			gLua->Call(7, 1);*/

			gLua->Call(6, 1);
		hookM->UnReference();
	hookT->UnReference();

	ILuaObject *ret = gLua->GetReturn(0);
		bool isNil = ret->isNil();
	ret->UnReference();

	if (isNil)
		return oldSpewFunction(spewType, msg);
	else
		return SPEW_CONTINUE;
}

int Open(lua_State *L)
{
	gLua = Lua();

	oldSpewFunction = GetSpewOutputFunc();

	SpewOutputFunc(SpewFunction);

	return 0;
}

int Close(lua_State *L)
{
	SpewOutputFunc(oldSpewFunction);

	return 0;
}