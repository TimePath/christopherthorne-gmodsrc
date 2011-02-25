#ifndef SOURCENET3_H
#define SOURCENET3_H

// GMod interface //

#include "GMLuaModule.h"

#define SOURCENET_META_BASE 5000

// Misc //

#define VerifyStream( stream ) \
	if ( stream < 0 || stream >= MAX_STREAMS ) \
	{ \
		Lua()->PushNil(); \
		return 1; \
	}

#define VerifyOffset( vec, offset ) \
	if ( offset < 0 || offset >= vec.Count() ) \
	{ \
		Lua()->PushNil(); \
		return 1; \
	}

#define CopyUserDataOrNull( arg, meta, dst, dsttype ) \
	int arg___t = Lua()->GetType( arg ); \
	if ( arg___t == GET_META_ID( meta ) ) \
		dst = (dsttype)GET_META( arg, meta ); \
	else if ( arg___t == GLua::TYPE_NUMBER && Lua()->GetInteger( arg ) == 0 ) \
		dst = (dsttype)NULL; \
	else \
		Lua()->TypeError( GET_META_NAME( meta ), arg )

// Multiple Lua state support //

#include <utllinkedlist.h>

typedef CUtlLinkedList<lua_State *> luaStateList_t;

luaStateList_t *GetLuaStates( void );

#define BEGIN_MULTISTATE_HOOK( name ) \
{ \
	luaStateList_t *states = GetLuaStates(); \
	if ( states ) \
	{ \
		for ( int i = 0; i < states->Count(); i++ ) \
		{ \
			lua_State *L = states->Element( i ); \
			if ( !L ) { Msg( "Lua state %i = NULL\n", i ); continue; } \
			UsesLua(); \
			ILuaObject *_G_hook = Lua()->GetGlobal( "hook" ); \
			ILuaObject *_G_hook_Call = _G_hook->GetMember( "Call" ); \
			Lua()->Push( _G_hook_Call ); \
			Lua()->Push( name ); \
			Lua()->PushNil(); \
			int argc = 0

#define DO_MULTISTATE_HOOK( code ) \
			code; \
			argc++

#define CALL_MULTISTATE_HOOK( returns ) \
			Lua()->Call( 2 + argc, returns )

#define STOP_MULTISTATE_HOOK() \
			_G_hook_Call->UnReference(); \
			_G_hook->UnReference(); \
			break;

#define END_MULTISTATE_HOOK() \
			_G_hook_Call->UnReference(); \
			_G_hook->UnReference(); \
		} \
	} \
	else \
	{ \
		Msg( "GetLuaStates() returned NULL\n" ); \
	} \
}

// ILuaInterface macros //

#undef	Lua
#define Lua()		pLuaInterface
#define UsesLua()	ILuaInterface *pLuaInterface = modulemanager->GetLuaInterface( L ) // (May not make a difference, compiler could optimize automatically?)

// Global macros //

// Meta tables (META_)

#define GET_META( index, name )					(name *)Lua()->GetUserData( index )

#define PUSH_META( data, name ) \
	{ \
		if ( data ) \
		{ \
			ILuaObject *META__tbl = Lua()->GetMetaTable( GET_META_NAME( name ), GET_META_ID( name ) ); \
			Lua()->PushUserData( META__tbl, (void *)data); \
			META__tbl->UnReference(); \
		} \
		else \
		{ \
			Lua()->PushNil(); \
		} \
	} \

#define META_FUNCTION( meta, name )				LUA_FUNCTION( meta##__##name )

#define	META_ID( name, id )						const int META_##name##_id = SOURCENET_META_BASE + id; \
												const char *META_##name##_name = #name

#define EXT_META_FUNCTION( meta, name)			extern META_FUNCTION( meta, name )

#define EXT_META_ID( name, id )					extern const int META_##name##_id; \
												extern const char *META_##name##_name

#define GET_META_ID( name )						META_##name##_id
#define GET_META_NAME( name )					META_##name##_name

#define BEGIN_META_REGISTRATION( name ) \
	{ \
		ILuaObject *META__tbl = Lua()->GetMetaTable( GET_META_NAME( name ), GET_META_ID( name ) ); \
		ILuaObject *META__index = Lua()->GetNewTable();

#define REG_META_FUNCTION( meta, name ) \
	META__index->SetMember( #name, meta##__##name )

#define REG_META_CALLBACK( meta, name ) \
	META__tbl->SetMember( #name, meta##__##name )

#define REG_META_CALLBACK_( meta, name, idx ) \
	META__tbl->SetMember( #idx, name )

#define END_META_REGISTRATION( ) \
		META__tbl->SetMember( "__index", META__index ); \
		META__index->UnReference(); \
		META__tbl->UnReference(); \
	} \

// Enumerations

#define BEGIN_ENUM_REGISTRATION( name ) \
	{ \
		Lua()->NewGlobalTable( #name ); \
		ILuaObject *ENUM__tbl = Lua()->GetGlobal( #name )

#define REG_ENUM( name, value ) \
	ENUM__tbl->SetMember( #value, (float)value )

#define END_ENUM_REGISTRATION( ) \
		ENUM__tbl->UnReference(); \
	} \

// Globals (GLBL_)

#define GLBL_FUNCTION( name )				LUA_FUNCTION( _G__##name )

#define EXT_GLBL_FUNCTION( name )			extern GLBL_FUNCTION( name )

#define REG_GLBL_FUNCTION( name )		Lua()->SetGlobal( #name, _G__##name )

#define REG_GLBL_NUMBER( name )		Lua()->SetGlobal( #name, (float)name )
#define REG_GLBL_STRING( name )		Lua()->SetGlobal( #name, (const char *)name )

// Interfaces //

#include <interface.h>

extern CreateInterfaceFn fnEngineFactory;

#ifdef IVENGINESERVER_INTERFACE

#include <eiface.h>

extern IVEngineServer *g_pEngineServer;

#endif

#ifdef IVENGINECLIENT_INTERFACE

#include <cdll_client_int.h>

extern IVEngineClient *g_pEngineClient;

#endif

#ifdef ICVAR_INTERFACE

#include <icvar.h>

extern ICvar *g_pCVarClient;
extern ICvar *g_pCVarServer;

#endif

// Cross-platform //

#ifdef _WIN32

#define ENGINE_LIB "engine.dll"
#define CLIENT_LIB "client.dll"
#define SERVER_LIB "server.dll"

#define CNetChan_ProcessMessages_SIG "\x83\xEC\x34\x53\x55\x89\x4C\x24\x08\x56"
#define CNetChan_ProcessMessages_MSK "xxxxxxxxxx"

#else

#define GetModuleHandle(name) dlopen(name, RTLD_LAZY)

#define ENGINE_LIB "engine.so"
#define CLIENT_LIB NULL
#define SERVER_LIB "server.so"

#endif

#endif // SOURCENET3_H
