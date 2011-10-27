//8B 4C 24 08 81 EC 00 02 00 00 8D 84 24 0C 02 00 00 50 51 8D 54 24 08 68 00 02 00 00 52 E8 1E 10
//02 00 8B 84 24 14 02 00 00 8B 88 8C 10 00 00 8B 11 8B 52 08

//8B 4C 24 08 81 EC 00 02 00 00 8D 84 24 0C 02 00 00 50 51 8D 54 24 08 68 00 02 00 00 52 E8 ?? ??
//?? ?? 8B 84 24 14 02 00 00 8B 88 8C 10 00 00 8B 11 8B 52 08

#define DRAWERROR_SIG "\x8B\x4C\x24\x08\x81\xEC\x00\x02\x00\x00\x8D\x84\x24\x0C\x02\x00\x00\x50\x51\x8D\x54\x24\x08\x68\x00\x02\x00\x00\x52\xE8\xCE\xFB\x01\x00\x8B\x84\x24\x14\x02\x00\x00\x8B\x88\x8C\x10\x00\x00\x8B\x11\x8B\x52\x08"
#define DRAWERROR_MASK "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxx"

#include "GMLuaModule.h"
#include "csimplescan.h"
#include "csimpledetour.h"
#include "cdetour.h"

#include <interface.h>
#include <windows.h>

GMOD_MODULE( Open, Close );

CSigScan DrawError_Sig;

SETUP_SIMPLE_DETOUR( DrawError_Detour, CDetour::DrawError_T, CDetour::DrawError_H );

ILuaInterface *g_Lua = NULL;

bool g_bLoaded = false;

void CDetour::DrawError_H( const char *err, ... )
{
	// Construct error string
	char szBuf[256];
	va_list arg_ptr;
	va_start( arg_ptr, err );
	_vsntprintf( szBuf, sizeof(szBuf)-1, (char *)err, arg_ptr );
	va_end( arg_ptr );

	// hook.Call
	ILuaObject *hookTable = g_Lua->GetGlobal( "hook" );
	ILuaObject *hookCallFunction = hookTable->GetMember( "Call" );
	hookCallFunction->Push();
	// Hook name
	g_Lua->Push( "LuaError" );
	// Gamemode (nil)
	g_Lua->PushNil();
	// Arguments
	g_Lua->Push(szBuf);
	// Call
	g_Lua->Call( 3, 1 );
	// Cleanup
	hookTable->UnReference();
	hookCallFunction->UnReference();

	// Get return value
	ILuaObject *returnValue = g_Lua->GetReturn( 0 );

	// Call original if nothing is returned
	if ( returnValue->isNil() )
		(this->*DrawError_T)( szBuf );

	// Cleanup
	returnValue->UnReference();
}

int Open( lua_State *L )
{
	if ( g_bLoaded )
	{
		Lua()->Error( "[gm_luaerror] Module can only be loaded in one state." );

		return 0;
	}

	CSimpleScan luaSharedInterface( "lua_shared.dll" );

	if ( luaSharedInterface.FindFunction( DRAWERROR_SIG, DRAWERROR_MASK, (void **)&CDetour::DrawError_T ) )
	{
		DrawError_Detour.Attach();
	}
	else
	{
		Lua()->Error( "[gm_luaerror] Signature scan failed. Report this please!" );

		return 0;
	}

	g_Lua = Lua();
	g_bLoaded = true;

	return 0;
}

int Close( lua_State *L )
{
	DrawError_Detour.Detach();

	return 0;
}