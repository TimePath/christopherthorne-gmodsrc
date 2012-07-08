#include "gm_luaerror.h"

GMOD_MODULE( Open, Close );

SETUP_SIMPLE_DETOUR( CLuaInterface_ErrorFromLua_Detour, CLuaInterface_Hooks::ErrorFromLua_T, CLuaInterface_Hooks::ErrorFromLua_H );

CUtlVector<ILuaInterface *> g_LuaInterfaces;

void CLuaInterface_Hooks::ErrorFromLua_H( const char *err, ... )
{
	ILuaInterface *luaInterface = (ILuaInterface *)this;

	// Construct error string
	char szBuf[512];
	va_list arg_ptr;
	va_start( arg_ptr, err );
	_vsntprintf( szBuf, sizeof(szBuf)-1, (char *)err, arg_ptr );
	va_end( arg_ptr );

	// Only call hook if the state has the module loaded
	if ( g_LuaInterfaces.Find( luaInterface ) == -1 )
	{
		(this->*ErrorFromLua_T)( szBuf );

		return;
	}

	// Call LuaError hook

	ILuaObject *hookTable = luaInterface->GetGlobal( "hook" );
	ILuaObject *hookCallFunction = hookTable->GetMember( "Call" );
	hookCallFunction->Push();

	luaInterface->Push( "LuaError" );
	luaInterface->PushNil();
	luaInterface->Push( szBuf );
	luaInterface->Call( 3, 1 );

	hookTable->UnReference();
	hookCallFunction->UnReference();

	// Check return value

	ILuaObject *returnValue = luaInterface->GetReturn( 0 );

	// Call original if nothing is returned
	if ( returnValue->isNil() )
		(this->*ErrorFromLua_T)( szBuf );

	returnValue->UnReference();
}

int Open( lua_State *L )
{
	CByteScanner luaSharedScanner( LUA_SHARED_LIBRARY );

	// Scan & hook if this is the first state loading the module
	if ( g_LuaInterfaces.Count() == 0 )
	{
		if ( luaSharedScanner.FindCodePattern( CLuaInterface_ErrorFromLua_Signature, CLuaInterface_ErrorFromLua_Mask, (LPVOID *)&CLuaInterface_Hooks::ErrorFromLua_T ) )
			CLuaInterface_ErrorFromLua_Detour.Attach();
		else
			Lua()->ErrorNoHalt( "[gm_luaerror] CLuaInterface::ErrorFromLua signature scan failed. Please report this error.\n" );
	}
	
	// Store state
	g_LuaInterfaces.AddToTail( Lua() );

	return 0;
}

int Close( lua_State *L )
{
	// Remove state
	g_LuaInterfaces.FindAndRemove( Lua() );

	// Detach if this is the last or only state unloading the module
	if ( g_LuaInterfaces.Count() == 0 )
		CLuaInterface_ErrorFromLua_Detour.Detach();

	return 0;
}