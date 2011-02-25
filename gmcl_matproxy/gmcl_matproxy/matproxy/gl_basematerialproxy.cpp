#include "gl_basematerialproxy.h"
#include "gl_materialproxies.h"

META_ID( CBaseMaterialProxy, 3 );

META_FUNCTION( CBaseMaterialProxy, __index )
{
	g_Lua->CheckType( 1, GET_META_ID( CBaseMaterialProxy ) );

	CBaseMaterialProxy *pProxy = GET_META( 1, CBaseMaterialProxy );

	g_Lua->PushReference( g_CurrentCallbackTable );

	ILuaObject *pCallbackTable = g_Lua->GetObject();

	g_Lua->Pop();

	if ( pCallbackTable )
	{
		bool bFound = false;

		ILuaObject *k = g_Lua->GetObject( 2 );
		ILuaObject *v = pCallbackTable->GetMember( k );
		
		k->UnReference();

		if ( v )
		{
			v->Push();
			v->UnReference();

			bFound = true;
		}

		pCallbackTable->UnReference();

		if ( bFound )
			return 1;
	}

	ILuaObject *pProxyVars = g_Lua->GetGlobal( "MatProxyVars" );

	if ( !pProxyVars->isNil() )
	{
		ILuaObject *pProxyIndex = pProxyVars->GetMember( (float)(unsigned long)pProxy );

		if ( pProxyIndex )
		{
			ILuaObject *k = g_Lua->GetObject( 2 );
			ILuaObject *v = pProxyIndex->GetMember( k );

			k->UnReference();

			if ( v )
			{
				v->Push();
				v->UnReference();
			}

			pProxyIndex->UnReference();
		}
	}

	pProxyVars->UnReference();

	return 1;
}

META_FUNCTION( CBaseMaterialProxy, __newindex )
{
	g_Lua->CheckType( 1, GET_META_ID( CBaseMaterialProxy ) );

	CBaseMaterialProxy *pProxy = GET_META( 1, CBaseMaterialProxy );

	ILuaObject *pProxyVars = g_Lua->GetGlobal( "MatProxyVars" );

	if ( pProxyVars->isNil() )
	{
		pProxyVars->UnReference();

		g_Lua->NewGlobalTable( "MatProxyVars" );

		pProxyVars = g_Lua->GetGlobal( "MatProxyVars" );
	}

	ILuaObject *pProxyIndex = pProxyVars->GetMember( (float)(unsigned long)pProxy );

	if ( !pProxyIndex )
	{
		pProxyIndex = g_Lua->GetNewTable();

		pProxyVars->SetMember( (float)(unsigned long)pProxy, pProxyIndex );
	}

	pProxyVars->UnReference();

	ILuaObject *k = g_Lua->GetObject( 2 );
	ILuaObject *v = g_Lua->GetObject( 3 );

	pProxyIndex->SetMember( k, v );
	pProxyIndex->UnReference();

	return 0;
}