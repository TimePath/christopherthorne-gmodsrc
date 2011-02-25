#include "gl_materialproxies.h"
#include "gl_basematerialproxy.h"
#include "gl_keyvalues.h"
#include "main.h"

#define CLIENT_DLL

#include "predictable_entity.h"
#include "c_baseentity.h"
#include "imaterialproxy.h"

int g_CurrentCallbackTable = -1;

CUtlVector<int> g_LuaMaterialReferences;

#define ENTER_CALLBACK( callback ) g_CurrentCallbackTable = callback->m_Table
#define EXIT_CALLBACK() g_CurrentCallbackTable = -1

C_BaseEntity *BindArgToEntity( void *pArg )
{
	IClientRenderable *pRend = (IClientRenderable *)pArg;
	return pRend->GetIClientUnknown()->GetBaseEntity();
}

///////////////////////////////////////////
/////////// Proxy Object Base /////////////
///////////////////////////////////////////

const char *CLuaMaterialProxyObject::GetName()
{
	return m_szName;
}

void CLuaMaterialProxyObject::SetName( const char *pszName )
{
	strcpy_s( m_szName, sizeof( m_szName ), pszName );
}

void CLuaMaterialProxyObject::Release()
{
	delete this;
}

///////////////////////////////////////////
//////// CLuaMaterialProxyCallback ////////
///////////////////////////////////////////

void CLuaMaterialProxyCallback::CreateReferences( ILuaObject *metaCallback )
{
	DevMsg( 5, "Creating CLuaMaterialProxyCallback (%s) references..\n", GetName() );

	UTIL_CreateLuaReference( metaCallback->GetMember( "Init" ), &m_fnInit );
	UTIL_CreateLuaReference( metaCallback->GetMember( "OnBind" ), &m_fnOnBind );
	UTIL_CreateLuaReference( metaCallback->GetMember( "GetMaterial" ), &m_fnGetMaterial );
	UTIL_CreateLuaReference( metaCallback, &m_Table );
}

void CLuaMaterialProxyCallback::FreeReferences()
{
	DevMsg( 5, "Deleting CLuaMaterialProxyCallback (%s) references..\n", GetName() );

	g_Lua->FreeReference( m_fnInit );
	g_Lua->FreeReference( m_fnOnBind );
	g_Lua->FreeReference( m_fnGetMaterial );
	g_Lua->FreeReference( m_Table );
}

///////////////////////////////////////////
//////////// CLuaMaterialProxy ////////////
///////////////////////////////////////////

void CLuaMaterialProxy::Add( CLuaMaterialProxyCallback *pCallback )
{
	m_Callbacks.AddToTail( pCallback );
}

void CLuaMaterialProxy::Remove( CLuaMaterialProxyCallback *pCallback )
{
	m_Callbacks.FindAndRemove( pCallback );

	pCallback->FreeReferences();
	pCallback->Release();

	if ( m_Callbacks.Count() <= 0 )
	{
		DevMsg( 5, "No callbacks left (%s), removing from g_LuaMaterialProxies..\n", GetName() );

		g_LuaMaterialProxies.FindAndRemove( this );

		Release();
	}
}

///////////////////////////////////////////
/////////// CBaseMaterialProxy ////////////
///////////////////////////////////////////

CBaseMaterialProxy::CBaseMaterialProxy( CLuaMaterialProxy *pLuaProxy )
{
	m_pLuaProxy = pLuaProxy;
}

bool CBaseMaterialProxy::Init( IMaterial *pMaterial, KeyValues *pKeyValues )
{
	DevMsg( 5, "CBaseMaterialProxy::Init( pMaterial = %p, pKeyValues = %p )\n", pMaterial, pKeyValues );

	for ( int i = 0; i < m_pLuaProxy->m_Callbacks.Count(); i++ )
	{
		CLuaMaterialProxyCallback *pCallback = m_pLuaProxy->m_Callbacks.Element( i );
		
		if ( pCallback->m_fnInit == -1 )
			continue;

		int nMaterialRef = -1;

		ILuaObject *metaIMaterial = g_Lua->GetMetaTable( "IMaterial", GLua::TYPE_MATERIAL );
		
		if ( metaIMaterial )
		{
			g_Lua->PushUserData( metaIMaterial, pMaterial ); // Push userdata
			ILuaObject *pMaterialObject = g_Lua->GetObject(); // Get as ILuaObject
			g_Lua->Pop(); // Pop userdata
			
			UTIL_CreateLuaReference( pMaterialObject, &nMaterialRef );
			
			g_LuaMaterialReferences.AddToTail( nMaterialRef ); // Free reference at gmod_close

			metaIMaterial->UnReference();
		}

		g_Lua->PushReference( pCallback->m_fnInit ); // Function
		
		PUSH_META( this, CBaseMaterialProxy ); // arg1

		g_Lua->PushReference( nMaterialRef ); // arg2

		PUSH_META( pKeyValues, KeyValues ); // arg3

		ENTER_CALLBACK( pCallback );

		g_Lua->Call( 3, 1 );

		EXIT_CALLBACK();

		ILuaObject *pReturn = g_Lua->GetReturn( 0 );

		bool bReturn = false; // If the callback returned false, we return false
		
		if ( pReturn->GetType() == GLua::TYPE_BOOL && pReturn->GetBool() == false )
			bReturn = true;

		pReturn->UnReference();

		if ( bReturn )
			return false;
	}

	return true;
}

void CBaseMaterialProxy::OnBind( void *pArg )
{
	//DevMsg( 5, "CBaseMaterialProxy::OnBind( pArg = %p ): proxyName = %s\n", pArg, m_pLuaProxy->GetName() );

	C_BaseEntity *pEntity = pArg ? BindArgToEntity( pArg ) : NULL; // This only supports entity and NULL args (If there are more (unlikely?), create userdata that allows user to cast to the correct type)

	for ( int i = 0; i < m_pLuaProxy->m_Callbacks.Count(); i++ )
	{
		CLuaMaterialProxyCallback *pCallback = m_pLuaProxy->m_Callbacks.Element( i );

		if ( pCallback->m_fnOnBind == -1 )
			continue;

		g_Lua->PushReference( pCallback->m_fnOnBind ); // Function
		
		PUSH_META( this, CBaseMaterialProxy ); // arg1

		if ( pEntity )
		{
			PUSH_META_REF( new CBaseHandle( pEntity->GetRefEHandle() ), g_EntityMetaTable ); // arg2 (Fucks with g_ActiveEHandles, but I can't think of a cleaner alternative)
		}
		else
		{
			g_Lua->PushNil();
		}

		ENTER_CALLBACK( pCallback );

		g_Lua->Call( 2, 0 );

		EXIT_CALLBACK();
	}
}

IMaterial *CBaseMaterialProxy::GetMaterial()
{
	DevMsg( 5, "CBaseMaterialProxy::GetMaterial(): proxyName = %s\n", m_pLuaProxy->GetName() );

	for ( int i = 0; i < m_pLuaProxy->m_Callbacks.Count(); i++ )
	{
		CLuaMaterialProxyCallback *pCallback = m_pLuaProxy->m_Callbacks.Element( i );

		if ( pCallback->m_fnGetMaterial == -1 )
			continue;

		g_Lua->PushReference( pCallback->m_fnGetMaterial ); // Function
		
		PUSH_META( this, CBaseMaterialProxy ); // arg1

		ENTER_CALLBACK( pCallback );

		g_Lua->Call( 1, 1 );

		EXIT_CALLBACK();

		IMaterial *pMaterial = NULL;

		ILuaObject *pReturn = g_Lua->GetReturn( 0 );

		if ( pReturn->GetType() == GLua::TYPE_MATERIAL )
			pMaterial = (IMaterial *)pReturn->GetUserData();

		pReturn->UnReference();

		if ( pMaterial )
			return pMaterial;
	}

	return NULL;
}

CUtlVector<CLuaMaterialProxy *> g_LuaMaterialProxies;

class CNewMaterialProxyFactoryOverride : public IMaterialProxyFactory
{
	IMaterialProxy *CreateProxy( const char *proxyName )
	{
		DevMsg( 5, "CreateProxy( proxyName = %s )\n", proxyName );

		for ( int i = 0; i < g_LuaMaterialProxies.Count(); i++ )
		{
			CLuaMaterialProxy *pLuaProxy = g_LuaMaterialProxies.Element( i );

			if ( V_strcmp( pLuaProxy->GetName(), proxyName ) == 0 )
				return new CBaseMaterialProxy( pLuaProxy );
		}

		return g_pOldProxyFactory->CreateProxy( proxyName );
	};

	void DeleteProxy( IMaterialProxy *pProxy )
	{
		DevMsg( 5, "DeleteProxy( pProxy = %p )\n", pProxy );

		if ( !pProxy )
			return;

		ILuaObject *pProxyVars = g_Lua->GetGlobal( "MatProxyVars" );

		if ( pProxyVars->isTable() )
		{
			ILuaObject *pProxyIndex = pProxyVars->GetMember( (float)(unsigned long)pProxy );

			if ( pProxyIndex )
			{
				DevMsg( 5, "Clearing %p vars from MatProxyVars table\n", pProxy );

				g_Lua->PushNil();
				ILuaObject *objNil = g_Lua->GetObject();
				g_Lua->Pop();

				pProxyVars->SetMember( (float)(unsigned long)pProxy, objNil );

				objNil->UnReference();

				pProxyIndex->UnReference();
			}
		}

		pProxyVars->UnReference();

		g_pOldProxyFactory->DeleteProxy( pProxy );
	};
};

CNewMaterialProxyFactoryOverride newProxyFactory;

IMaterialProxyFactory *g_pNewProxyFactory = (IMaterialProxyFactory *)&newProxyFactory;