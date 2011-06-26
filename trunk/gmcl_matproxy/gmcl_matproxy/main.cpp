// Headers

#include "GMLuaModuleEx.h"
#include "gl_keyvalues.h"
#include "gl_materialproxies.h"
#include "gl_imaterial_ex.h"
#include "gl_imaterialvar.h"
#include "gl_basematerialproxy.h"
#include "utlvector.h"
#include "main.h"

// Platform Definitions

#ifdef _WIN32
#define MATERIALSYSTEM_LIB "materialsystem.dll"
#endif

// Lua Interface

ILuaInterface *g_Lua = NULL;

// Lua References

int g_EntityMetaTable = -1;

// Material System

IMaterialSystem *g_pMaterialSystem = NULL;

// Material Proxy

IMaterialProxyFactory *g_pOldProxyFactory = NULL;

// Material Proxy Registration

LUA_FUNCTION( matproxy_Add )
{
	g_Lua->CheckType( 1, GLua::TYPE_STRING );
	g_Lua->CheckType( 2, GLua::TYPE_STRING );
	g_Lua->CheckType( 3, GLua::TYPE_TABLE );

	const char *proxyName = g_Lua->GetString( 1 );
	const char *callbackName = g_Lua->GetString( 2 );

	CLuaMaterialProxy *pLuaProxy = NULL;
	CLuaMaterialProxyCallback *pCallback = NULL;

	bool bProxyExists = false;
	bool bCallbackExists = false;

	for ( int i = 0; i < g_LuaMaterialProxies.Count(); i++ )
	{
		pLuaProxy = g_LuaMaterialProxies.Element( i );

		if ( V_strcmp( pLuaProxy->GetName(), proxyName ) == 0 ) // Check if proxy type is already registered
		{
			DevMsg( 5, "(matproxy.Add) Found existing proxy for %s\n", proxyName );

			for ( int j = 0; j < pLuaProxy->m_Callbacks.Count(); j++ )
			{
				pCallback = pLuaProxy->m_Callbacks.Element( j );

				if ( V_strcmp( pCallback->GetName(), callbackName ) == 0 ) // Check if a proxy is registered with this ID
				{
					DevMsg( 5, "(matproxy.Add) Found existing callbacks for %s/%s\n", proxyName, callbackName );

					pLuaProxy->m_Callbacks.Remove( j );

					pCallback->FreeReferences();

					bCallbackExists = true;
					
					break;
				}
			}

			bProxyExists = true;

			break;
		}
	}

	if ( !bProxyExists )
	{
		pLuaProxy = new CLuaMaterialProxy;

		pLuaProxy->SetName( proxyName );

		g_LuaMaterialProxies.AddToTail( pLuaProxy );
	}

	if ( !bCallbackExists )
	{
		pCallback = new CLuaMaterialProxyCallback;

		pCallback->SetName( callbackName );
	}

	pCallback->CreateReferences( g_Lua->GetObject( 3 ) );

	pLuaProxy->Add( pCallback );

	return 0;
}

LUA_FUNCTION( matproxy_Remove )
{
	g_Lua->CheckType( 1, GLua::TYPE_STRING );
	g_Lua->CheckType( 2, GLua::TYPE_STRING );

	const char *proxyName = g_Lua->GetString( 1 );
	const char *callbackName = g_Lua->GetString( 2 );

	for ( int i = 0; i < g_LuaMaterialProxies.Count(); i++ )
	{
		CLuaMaterialProxy *pLuaProxy = g_LuaMaterialProxies.Element( i );

		if ( V_strcmp( pLuaProxy->GetName(), proxyName ) == 0 )
		{
			DevMsg( 5, "(matproxy.Remove) Found existing proxy for %s\n", proxyName );

			for ( int j = 0; j < pLuaProxy->m_Callbacks.Count(); j++ )
			{
				CLuaMaterialProxyCallback *pCallback = pLuaProxy->m_Callbacks.Element( j );

				if ( V_strcmp( pCallback->GetName(), callbackName ) == 0 )
				{
					DevMsg( 5, "(matproxy.Remove) Found existing callbacks for %s/%s\n", proxyName, callbackName );

					pLuaProxy->Remove( pCallback );

					g_Lua->Push( true );

					return 1;
				}
			}
		}
	}

	g_Lua->Push( false );

	return 1;
}

// Material Reloading

LUA_FUNCTION( matsystem_ReloadMaterials )
{
	g_pMaterialSystem->ReloadMaterials( g_Lua->GetString( 1 ) );

	return 0;
}

// Hacks!

bool HACK_IsMenu( ILuaInterface *i )
{
	ILuaObject *fnLocalPlayer = i->GetGlobal( "LocalPlayer" );

	bool bReturn = fnLocalPlayer->isNil();

	fnLocalPlayer->UnReference();

	return bReturn;
}

// Initialize

void Open( ILuaInterface *i )
{
	if ( HACK_IsMenu( i ) )
	{
		i->Error( "Can't load gmcl_matproxy in the menu environment" );

		return;
	}

	g_Lua = i;

	CreateInterfaceFn fnMaterialSystemFactory = Sys_GetFactory( MATERIALSYSTEM_LIB );

	if ( !fnMaterialSystemFactory )
	{
		g_Lua->Error( "Failed to retrieve material system factory" );

		return;
	}

	g_pMaterialSystem = (IMaterialSystem *)fnMaterialSystemFactory( MATERIAL_SYSTEM_INTERFACE_VERSION, NULL );

	if ( !g_pMaterialSystem )
	{
		g_Lua->Error( "Failed to retrieve material system interface" );
		
		return;
	}

	g_pOldProxyFactory = g_pMaterialSystem->GetMaterialProxyFactory(); // Save old proxy factory

	if ( !g_pOldProxyFactory )
	{
		g_Lua->Error( "Failed to retrieve current material proxy factory" );

		return;
	}

	g_pMaterialSystem->SetMaterialProxyFactory( g_pNewProxyFactory ); // Set new proxy factory

	// Lua References

	UTIL_CreateLuaReference( g_Lua->GetMetaTable( "Entity", GLua::TYPE_ENTITY ), &g_EntityMetaTable );

	// KeyValues metatable

	BEGIN_META_REGISTRATION( KeyValues );
		REG_META_FUNCTION( KeyValues, GetName );
		REG_META_FUNCTION( KeyValues, SetName );
		REG_META_FUNCTION( KeyValues, FindKey );
		REG_META_FUNCTION( KeyValues, CreateNewKey );
		REG_META_FUNCTION( KeyValues, AddSubKey );
		REG_META_FUNCTION( KeyValues, RemoveSubKey );
		REG_META_FUNCTION( KeyValues, GetFirstSubKey );
		REG_META_FUNCTION( KeyValues, GetNextKey );
		REG_META_FUNCTION( KeyValues, SetNextKey );
		REG_META_FUNCTION( KeyValues, GetFirstTrueSubKey );
		REG_META_FUNCTION( KeyValues, GetNextTrueSubKey );
		REG_META_FUNCTION( KeyValues, GetFirstValue );
		REG_META_FUNCTION( KeyValues, GetNextValue );
		REG_META_FUNCTION( KeyValues, GetInt );
		REG_META_FUNCTION( KeyValues, SetInt );
		REG_META_FUNCTION( KeyValues, GetFloat );
		REG_META_FUNCTION( KeyValues, SetFloat );
		REG_META_FUNCTION( KeyValues, GetString );
		REG_META_FUNCTION( KeyValues, SetString );
		REG_META_FUNCTION( KeyValues, GetColor );
		REG_META_FUNCTION( KeyValues, SetColor );
		REG_META_FUNCTION( KeyValues, IsEmpty );
	END_META_REGISTRATION();

	// IMaterial metatable extension

	BEGIN_META_GEXTENSION( "IMaterial", GLua::TYPE_MATERIAL );
		REG_META_GFUNCTION( IMaterial, FindVar );
	END_META_GEXTENSION();

	// IMaterialVar metatable

	BEGIN_META_REGISTRATION( IMaterialVar );
		REG_META_FUNCTION( IMaterialVar, GetName );
		REG_META_FUNCTION( IMaterialVar, GetTextureValue );
		REG_META_FUNCTION( IMaterialVar, SetTextureValue );
		REG_META_FUNCTION( IMaterialVar, GetFloatValue );
		REG_META_FUNCTION( IMaterialVar, SetFloatValue );
		REG_META_FUNCTION( IMaterialVar, GetIntValue );
		REG_META_FUNCTION( IMaterialVar, SetIntValue );
		REG_META_FUNCTION( IMaterialVar, GetStringValue );
		REG_META_FUNCTION( IMaterialVar, SetStringValue );
		REG_META_FUNCTION( IMaterialVar, GetMaterialValue );
		REG_META_FUNCTION( IMaterialVar, SetMaterialValue );
		REG_META_FUNCTION( IMaterialVar, GetVecValue );
		REG_META_FUNCTION( IMaterialVar, SetVecValue );
		REG_META_FUNCTION( IMaterialVar, GetMatrixValue );
		REG_META_FUNCTION( IMaterialVar, SetMatrixValue );
		REG_META_FUNCTION( IMaterialVar, SetVecComponentValue );
		REG_META_FUNCTION( IMaterialVar, VectorSize );
		REG_META_FUNCTION( IMaterialVar, IsDefined );
		REG_META_FUNCTION( IMaterialVar, SetUndefined );
		REG_META_FUNCTION( IMaterialVar, GetOwningMaterial );
		REG_META_FUNCTION( IMaterialVar, GetType );
		REG_META_FUNCTION( IMaterialVar, IsTexture );
	END_META_REGISTRATION();

	// CBaseMaterialProxy (no __index table)

	_BEGIN_META_REGISTRATION( CBaseMaterialProxy );
		REG_META_CALLBACK( CBaseMaterialProxy, __index );
		REG_META_CALLBACK( CBaseMaterialProxy, __newindex );
	_END_META_REGISTRATION();

	// matproxy global table

	BEGIN_GTABLE_REGISTRATION( matproxy );
		REG_GTABLE_FUNCTION( matproxy, Add );
		REG_GTABLE_FUNCTION( matproxy, Remove );
	END_GTABLE_REGISTRATION();

	// matsystem global table

	BEGIN_GTABLE_REGISTRATION( matsystem );
		REG_GTABLE_FUNCTION( matsystem, ReloadMaterials );
	END_GTABLE_REGISTRATION();

	// MaterialVarType_t enum

	REG_GLBL_NUMBER( MATERIAL_VAR_TYPE_FLOAT );
	REG_GLBL_NUMBER( MATERIAL_VAR_TYPE_STRING );
	REG_GLBL_NUMBER( MATERIAL_VAR_TYPE_VECTOR );
	REG_GLBL_NUMBER( MATERIAL_VAR_TYPE_TEXTURE );
	REG_GLBL_NUMBER( MATERIAL_VAR_TYPE_INT );
	REG_GLBL_NUMBER( MATERIAL_VAR_TYPE_FOURCC );
	REG_GLBL_NUMBER( MATERIAL_VAR_TYPE_UNDEFINED );
	REG_GLBL_NUMBER( MATERIAL_VAR_TYPE_MATRIX );
	REG_GLBL_NUMBER( MATERIAL_VAR_TYPE_MATERIAL );

	// KeyValues types_t

	_REG_GLBL_NUMBER( "KEYVALUES_TYPE_NONE", KeyValues::TYPE_NONE );
	_REG_GLBL_NUMBER( "KEYVALUES_TYPE_STRING", KeyValues::TYPE_STRING );
	_REG_GLBL_NUMBER( "KEYVALUES_TYPE_INT", KeyValues::TYPE_INT );
	_REG_GLBL_NUMBER( "KEYVALUES_TYPE_FLOAT", KeyValues::TYPE_FLOAT );
	_REG_GLBL_NUMBER( "KEYVALUES_TYPE_PTR", KeyValues::TYPE_PTR );
	_REG_GLBL_NUMBER( "KEYVALUES_TYPE_WSTRING", KeyValues::TYPE_WSTRING );
	_REG_GLBL_NUMBER( "KEYVALUES_TYPE_COLOR", KeyValues::TYPE_COLOR );
	_REG_GLBL_NUMBER( "KEYVALUES_TYPE_UINT64", KeyValues::TYPE_UINT64 );
	_REG_GLBL_NUMBER( "KEYVALUES_TYPE_NUMTYPES", KeyValues::TYPE_NUMTYPES );
}

extern "C" int GM_EXPORT gmod_open( ILuaInterface *i )
{
	if ( i )
		Open( i );

	return 0;
}

// Shutdown

void Close( lua_State *L )
{
	if ( g_pMaterialSystem )
		g_pMaterialSystem->SetMaterialProxyFactory( g_pOldProxyFactory ); // Revert to old proxy factory

	for ( int i = g_LuaMaterialProxies.Count(); --i >= 0; )
	{
		CLuaMaterialProxy *pLuaProxy = g_LuaMaterialProxies.Element( i );

		for ( int j = pLuaProxy->m_Callbacks.Count(); --j >= 0; )
		{
			CLuaMaterialProxyCallback *pCallback = pLuaProxy->m_Callbacks.Element( j );

			pCallback->FreeReferences();
			pCallback->Release();

			pLuaProxy->m_Callbacks.Remove( j );
		}

		pLuaProxy->Release();

		g_LuaMaterialProxies.Remove( i );
	}

	g_Lua->FreeReference( g_EntityMetaTable );

	for ( int i = g_LuaMaterialReferences.Count(); --i >= 0; )
	{
		g_Lua->FreeReference( g_LuaMaterialReferences.Element( i ) );

		g_LuaMaterialReferences.Remove( i );
	}
}

extern "C" int GM_EXPORT gmod_close( lua_State *L )
{
	Close( L );

	return 0;
}