// SDK
#include <tier0/dbg.h>

// GMod
#include "GMLuaModuleEx.h"

// GMod entry points
GMOD_MODULE( Open, Close );

// Previous spew function
SpewOutputFunc_t g_fnOldSpew = NULL;

// New spew function
SpewRetval_t LuaSpew( SpewType_t spewType, const char *pMsg )
{
	// Check for NULL pointer

	if ( !pMsg )
		return SPEW_CONTINUE;

	// Begin hook.Call
	
	BEGIN_MULTISTATE_HOOK( "EngineSpew" );
	
	// Push arguments
	
	DO_MULTISTATE_HOOK( Lua()->PushLong( spewType ) );
	DO_MULTISTATE_HOOK( Lua()->Push( pMsg ) );
	DO_MULTISTATE_HOOK( Lua()->Push( GetSpewOutputGroup() ) );
	DO_MULTISTATE_HOOK( Lua()->PushLong( GetSpewOutputLevel() ) );
	
	// Invoke hook.Call	
	
	CALL_MULTISTATE_HOOK( 1 );
	
	// Check return value

	ILuaObject *_return = Lua()->GetReturn( 0 );

	bool isNil = _return->isNil();
			
	_return->UnReference();

	if ( !isNil )
		return SPEW_CONTINUE;

	// Finish hook.Call

	END_MULTISTATE_HOOK();

	// Pass to default handler

	return g_fnOldSpew( spewType, pMsg );
}

// Module load
int Open( lua_State *L )
{
	ADD_LUA_STATE( L );

	g_fnOldSpew = GetSpewOutputFunc();

	SpewOutputFunc( LuaSpew );

	return 0;
}

// Module shutdown
int Close( lua_State *L )
{
	REMOVE_LUA_STATE( L );

	SpewOutputFunc( g_fnOldSpew );

	return 0;
}