#include "gmsv_rcon.h"

_PREPARE(CServerRemoteAccess, WriteDataRequest);
_PREPARE(CServerRemoteAccess, LogCommand);
_PREPARE(CServerRemoteAccess, CheckPassword);

_PREPARE(CRConServer, IsPassword);

netadr_s *last_addr; //HACKY

netadr_s *ListenerIDToAddress(CServerRemoteAccess *c, unsigned int id) //TODO: Replace with CUtlLinkedList usage
{
	unsigned int pos = (id*8)-id;

	netadr_s *addr;

	__asm
	{
		PUSHAD
		MOV ECX, c
		MOV EAX, [ECX+0x28] //CUtlLinkedList
		MOV ECX, pos
		LEA EDX, [EAX+ECX*0x04]
		LEA EAX, [EDX+0x08] //netadr_s
		MOV addr, EAX
		POPAD
	}

	return addr;
}

_HOOK(CServerRemoteAccess, void, WriteDataRequest, (CRConServer *conn, unsigned int listener, const void *pData, int len))
{
	netadr_s *addr = ListenerIDToAddress((CServerRemoteAccess *)this, listener);

	bf_read buffer(pData, len);

	ILuaObject *hookT = gLua->GetGlobal("hook");
		ILuaObject *hookM = hookT->GetMember("Call");
			hookM->Push();

			gLua->Push("RCON_WriteDataRequest");
			gLua->PushNil();
			gLua->PushDouble(buffer.ReadLong());
			gLua->PushDouble(buffer.ReadLong());

			char *str = buffer.ReadAndAllocateString();

			gLua->Push(str);

			delete str;

			gLua->Push(addr->ToString(true));
			gLua->PushDouble(addr->GetPort());
			gLua->Call(7, 1);
		hookM->UnReference();
	hookT->UnReference();

	ILuaObject *ret = gLua->GetReturn(0);
		bool standard = ret->isNil();
	ret->UnReference();

	if (standard)
		(this->*CServerRemoteAccess_WriteDataRequest_T)(conn, listener, pData, len);
}

_HOOK(CServerRemoteAccess, void, LogCommand, (unsigned int listener, const char *msg))
{
	netadr_s *addr = ListenerIDToAddress((CServerRemoteAccess *)this, listener);

	ILuaObject *hookT = gLua->GetGlobal("hook");
		ILuaObject *hookM = hookT->GetMember("Call");
			hookM->Push();

			gLua->Push("RCON_LogCommand");
			gLua->PushNil();
			gLua->Push(msg);
			gLua->Push(addr->ToString(true));
			gLua->PushDouble(addr->GetPort());
			gLua->Call(5, 1);
		hookM->UnReference();
	hookT->UnReference();

	ILuaObject *ret = gLua->GetReturn(0);
		bool standard = ret->isNil();
	ret->UnReference();
	
	if (standard)
		(this->*CServerRemoteAccess_LogCommand_T)(listener, msg);
}

_HOOK(CServerRemoteAccess, void *, CheckPassword, (CRConServer *conn, unsigned int listener, int id, const char *password))
{
	last_addr = ListenerIDToAddress((CServerRemoteAccess *)this, listener);

	return (this->*CServerRemoteAccess_CheckPassword_T)(conn, listener, id, password);
}

_HOOK(CRConServer, bool, IsPassword, (const char *password))
{
	if (last_addr)
	{
		ILuaObject *hookT = gLua->GetGlobal("hook");
			ILuaObject *hookM = hookT->GetMember("Call");
				hookM->Push();

				gLua->Push("RCON_CheckPassword");
				gLua->PushNil();
				gLua->Push(password);
				gLua->Push(last_addr->ToString(true));
				gLua->PushDouble(last_addr->GetPort());
				gLua->Call(5, 1);
			hookM->UnReference();
		hookT->UnReference();

		ILuaObject *ret = gLua->GetReturn(0);
			bool isNil = ret->isNil();
			bool validation = ret->GetBool();
		ret->UnReference();

		last_addr = NULL;

		if (!isNil)
			return validation;
	}

	return (this->*CRConServer_IsPassword_T)(password);
}

int Open(lua_State *L)
{
	gLua = Lua();

	CreateInterfaceFn engineFactory = Sys_GetFactory("engine.dll");

	engine = (IVEngineServer *)engineFactory(INTERFACEVERSION_VENGINESERVER, NULL);

	if (!engine)
	{
		gLua->Error("[gmsv_rcon] Failed to retrieve engine interface");

		return 0;
	}

	CSigScan::sigscan_dllfunc = (CreateInterfaceFn)engine;

	if (!CSigScan::GetDllMemInfo())
	{
		gLua->Error("[gmsv_rcon] CSigScan::GetDllMemInfo() call failed");

		return 0;
	}

	_ATTACH(CServerRemoteAccess, WriteDataRequest, "\x8B\x44\x24\x10\x81\xEC\x70\x03\x00\x00", "xxxxxxxxxx", 10);
	_ATTACH(CServerRemoteAccess, LogCommand, "\xA1\x34\xE3\xEE\x0B\x83\x78\x30\x00\x74\x53\x8B\x44\x24\x04\x3B\x41\x40", "x????xxxxx?xxxxxxx", 18);
	_ATTACH(CServerRemoteAccess, CheckPassword, "\x8B\x44\x24\x10\x55\x56\x8B\x74\x24\x0C", "xxxxxxxxxx", 10);

	_ATTACH(CRConServer, IsPassword, "\x56\x8D\x71\x40\x8B\xCE", "xxxxxx", 6);

	return 0;
}

int Close(lua_State *L)
{
	_DETACH(CServerRemoteAccess, WriteDataRequest);
	_DETACH(CServerRemoteAccess, LogCommand);
	_DETACH(CServerRemoteAccess, CheckPassword);

	_DETACH(CRConServer, IsPassword);

	return 0;
}