#ifndef GMSV_RCON_H
#define GMSV_RCON_H

#define CRCONSERVER_NAME "CRConServer"
#define CRCONSERVER_ID 4567

#include "gminterface/include/GMLuaModule.h"
#include "sigscan/include/sigscan.h"
#include "detours/include/detours.h"

#include <eiface.h>
#include <bitbuf.h>
#include <utlbuffer.h>

#include "cdetour.h"

GMOD_MODULE(Open, Close);

ILuaInterface *gLua = NULL;
IVEngineServer *engine = NULL;

#define _PREPARE(c, name) \
	CSigScan c##_##name##_Sig

#define _HOOK(c, ret, name, args) \
	ret CDetour::##c##_##name##_H##args

#define _ATTACH(c, name, sig, mask, len) \
	c##_##name##_Sig.Init((unsigned char *)sig, mask, len); \
	if (c##_##name##_Sig.is_set) \
	{ \
	CDetour::##c##_##name##_T = *((##c##_##name##_t)&##c##_##name##_Sig.sig_addr); \
		DetourTransactionBegin(); \
		DetourUpdateThread(GetCurrentThread()); \
		DetourAttach(&(PVOID &)CDetour::##c##_##name##_T, (PVOID)(&(PVOID &)CDetour::##c##_##name##_H)); \
		DetourTransactionCommit(); \
	} \
	else \
	{ \
		gLua->Msg("[gmsv_rcon] Failed to retrieve address of %s::%s\n", #c, #name); \
	}

#define _DETACH(c, name) \
	if (##c##_##name##_Sig.is_set) \
	{ \
		DetourTransactionBegin(); \
		DetourUpdateThread(GetCurrentThread()); \
		DetourDetach(&(PVOID &)CDetour::##c##_##name##_T, (PVOID)(&(PVOID &)CDetour::##c##_##name##_H)); \
		DetourTransactionCommit(); \
	}

#endif //GMSV_RCON_H