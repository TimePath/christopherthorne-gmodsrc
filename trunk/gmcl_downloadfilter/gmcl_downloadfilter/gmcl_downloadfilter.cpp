#include "gmcl_downloadfilter.h"

bool ShouldDownload_H(const char *filename)
{
	ILuaObject *hookT = gLua->GetGlobal("hook");
		ILuaObject *hookM = hookT->GetMember("Call");
			hookM->Push();

			gLua->Push("ShouldDownload");
			gLua->PushNil();
			gLua->Push(filename);

			gLua->Call(3, 1);
		hookM->UnReference();
	hookT->UnReference();

	bool shouldDownload = false;

	ILuaObject *ret = gLua->GetReturn(0);
		if ((ret->isNil()) || (ret->GetBool() == true))
			shouldDownload = true;
	ret->UnReference();

	if (shouldDownload)
		return ShouldDownload_T(filename);
	else
		return shouldDownload;
}

int Open(lua_State *L)
{
	gLua = Lua();

	CreateInterfaceFn engineFactory = Sys_GetFactory("engine.dll");

	engine = (IVEngineServer *)engineFactory(INTERFACEVERSION_VENGINESERVER, NULL);
	
	if (!engine)
	{
		gLua->Error("Failed to retrieve IVEngineServer");

		return 0;
	}

	CSigScan::sigscan_dllfunc = (CreateInterfaceFn)engine;

	if (!CSigScan::GetDllMemInfo())
	{
		gLua->Error("CSigScan::GetDllMemInfo() call failed");

		return 0;
	}

	ShouldDownload_Sig.Init((unsigned char *)"\xF7\x05\x54\xCB\xD2\x0D\x00\x10\x00\x00\x74\x16\x8B\x4C\x24\x04", "xx????xxxxx?xxxx", 16);

	if (ShouldDownload_Sig.is_set)
	{
		ShouldDownload_T = (ShouldDownload_t)ShouldDownload_Sig.sig_addr;

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DetourAttach(&(PVOID &)ShouldDownload_T, (PVOID)(&(PVOID &)ShouldDownload_H));

		DetourTransactionCommit();
	}
	else
	{
		gLua->Error("ShouldDownload SigScan failed!");
	}

	return 0;
}

int Close(lua_State *L)
{
	if (ShouldDownload_Sig.is_set)
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DetourDetach(&(PVOID &)ShouldDownload_T, (PVOID)(&(PVOID &)ShouldDownload_H));

		DetourTransactionCommit();
	}

	return 0;
}