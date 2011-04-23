#ifndef GMLUAMODULEEX_H
#define GMLUAMODULEEX_H

// GMod interface

#include "GMLuaModule.h"

// Multiple Lua state support

#include <utllinkedlist.h>

struct multiStateInfo
{
	lua_State *L;
	int ref_hook_Call; // hook.Call
};

typedef CUtlLinkedList<multiStateInfo> luaStateList_t;

luaStateList_t g_LuaStates;

#define ADD_LUA_STATE( L ) \
{ \
	UsesLua(); \
	multiStateInfo msi; \
	msi.L = L; \
	ILuaObject *_G_hook = Lua()->GetGlobal( "hook" ); \
	ILuaObject *_G_hook_Call = _G_hook->GetMember( "Call" ); \
	_G_hook_Call->Push(); \
	msi.ref_hook_Call = Lua()->GetReference( -1, true ); \
	_G_hook_Call->UnReference(); \
	_G_hook->UnReference(); \
	g_LuaStates.AddToTail( msi ); \
}

#define REMOVE_LUA_STATE( L ) \
{ \
	UsesLua(); \
	for ( int i = 0; i < g_LuaStates.Count(); i++ ) \
	{ \
		multiStateInfo msi = g_LuaStates[i]; \
		if ( msi.L == L ) \
		{ \
			Lua()->FreeReference( msi.ref_hook_Call ); \
			g_LuaStates.Remove( i ); \
			break; \
		} \
	} \
}

inline luaStateList_t *GetLuaStates( void )
{
	return &g_LuaStates;
}

#define BEGIN_MULTISTATE_HOOK( name ) \
{ \
	luaStateList_t *states = GetLuaStates(); \
	if ( states ) \
	{ \
		for ( int i = 0; i < states->Count(); i++ ) \
		{ \
			multiStateInfo msi = states->Element( i ); \
			lua_State *L = msi.L; \
			UsesLua(); \
			Lua()->PushReference( msi.ref_hook_Call ); \
			Lua()->Push( name ); \
			Lua()->PushNil(); \
			int argc = 0

#define DO_MULTISTATE_HOOK( code ) \
			code; \
			argc++

#define CALL_MULTISTATE_HOOK( returns ) \
			Lua()->Call( 2 + argc, returns )

#define STOP_MULTISTATE_HOOK() \
			break;

#define END_MULTISTATE_HOOK() \
		} \
	} \
	else \
	{ \
		Msg( "GetLuaStates() returned NULL\n" ); \
	} \
}

// ILuaInterface macros

#undef Lua
#define Lua() pLuaInterface
#define UsesLua() ILuaInterface *pLuaInterface = modulemanager->GetLuaInterface( L )

#endif // GMLUAMODULEEX_H