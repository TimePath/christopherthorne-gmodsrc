#include "gl_keyvalues.h"

META_ID( KeyValues, 2 );

META_FUNCTION( KeyValues, GetName )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );

	KeyValues *kv = GET_META( 1, KeyValues );

	g_Lua->Push( kv->GetName() );

	return 1;
}

META_FUNCTION( KeyValues, SetName )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );
	g_Lua->CheckType( 2, GLua::TYPE_STRING );

	KeyValues *kv = GET_META( 1, KeyValues );

	kv->SetName( g_Lua->GetString( 2 ) );

	return 0;
}

META_FUNCTION( KeyValues, FindKey )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );
	g_Lua->CheckType( 2, GLua::TYPE_STRING );

	KeyValues *kv = GET_META( 1, KeyValues );

	PUSH_META( kv->FindKey( g_Lua->GetString( 2 ) ), KeyValues );

	return 1;
}

META_FUNCTION( KeyValues, CreateNewKey )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );

	KeyValues *kv = GET_META( 1, KeyValues );

	PUSH_META( kv->CreateNewKey(), KeyValues );

	return 1;
}

META_FUNCTION( KeyValues, AddSubKey )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );
	g_Lua->CheckType( 2, GET_META_ID( KeyValues ) );

	KeyValues *kv = GET_META( 1, KeyValues );

	kv->AddSubKey( GET_META( 2, KeyValues ) );

	return 0;
}

META_FUNCTION( KeyValues, RemoveSubKey )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );
	g_Lua->CheckType( 2, GET_META_ID( KeyValues ) );

	KeyValues *kv = GET_META( 1, KeyValues );

	kv->RemoveSubKey( GET_META( 2, KeyValues ) );

	return 0;
}

META_FUNCTION( KeyValues, GetFirstSubKey )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );

	KeyValues *kv = GET_META( 1, KeyValues );

	PUSH_META( kv->GetFirstSubKey(), KeyValues );

	return 1;
}

META_FUNCTION( KeyValues, GetNextKey )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );

	KeyValues *kv = GET_META( 1, KeyValues );

	PUSH_META( kv->GetNextKey(), KeyValues );

	return 1;
}

META_FUNCTION( KeyValues, SetNextKey )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );
	g_Lua->CheckType( 2, GET_META_ID( KeyValues ) );

	KeyValues *kv = GET_META( 1, KeyValues );

	kv->SetNextKey( GET_META( 2, KeyValues ) );

	return 0;
}

META_FUNCTION( KeyValues, GetFirstTrueSubKey )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );

	KeyValues *kv = GET_META( 1, KeyValues );

	PUSH_META( kv->GetFirstTrueSubKey(), KeyValues );

	return 1;
}

META_FUNCTION( KeyValues, GetNextTrueSubKey )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );

	KeyValues *kv = GET_META( 1, KeyValues );

	PUSH_META( kv->GetNextTrueSubKey(), KeyValues );

	return 1;
}

META_FUNCTION( KeyValues, GetFirstValue )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );

	KeyValues *kv = GET_META( 1, KeyValues );

	PUSH_META( kv->GetFirstValue(), KeyValues );

	return 1;
}

META_FUNCTION( KeyValues, GetNextValue )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );

	KeyValues *kv = GET_META( 1, KeyValues );

	PUSH_META( kv->GetNextValue(), KeyValues );

	return 1;
}

META_FUNCTION( KeyValues, GetInt )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );
	g_Lua->CheckType( 2, GLua::TYPE_STRING );

	KeyValues *kv = GET_META( 1, KeyValues );

	g_Lua->PushLong( kv->GetInt( g_Lua->GetString( 2 ) ) );

	return 1;
}

META_FUNCTION( KeyValues, SetInt )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );
	g_Lua->CheckType( 2, GLua::TYPE_STRING );
	g_Lua->CheckType( 3, GLua::TYPE_NUMBER );

	KeyValues *kv = GET_META( 1, KeyValues );

	kv->SetInt( g_Lua->GetString( 2 ), g_Lua->GetInteger( 3 ) );

	return 0;
}

META_FUNCTION( KeyValues, GetFloat )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );
	g_Lua->CheckType( 2, GLua::TYPE_STRING );

	KeyValues *kv = GET_META( 1, KeyValues );

	g_Lua->Push( kv->GetFloat( g_Lua->GetString( 2 ) ) );

	return 1;
}

META_FUNCTION( KeyValues, SetFloat )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );
	g_Lua->CheckType( 2, GLua::TYPE_STRING );
	g_Lua->CheckType( 3, GLua::TYPE_NUMBER );

	KeyValues *kv = GET_META( 1, KeyValues );

	kv->SetFloat( g_Lua->GetString( 2 ), g_Lua->GetNumber( 3 ) );

	return 0;
}

META_FUNCTION( KeyValues, GetString )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );
	g_Lua->CheckType( 2, GLua::TYPE_STRING );

	KeyValues *kv = GET_META( 1, KeyValues );

	const char *pszValue = kv->GetString( g_Lua->GetString( 2 ) );

	if ( !pszValue )
		g_Lua->PushNil();
	else
		g_Lua->Push( pszValue );

	return 1;
}

META_FUNCTION( KeyValues, SetString )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );
	g_Lua->CheckType( 2, GLua::TYPE_STRING );
	g_Lua->CheckType( 3, GLua::TYPE_STRING );

	KeyValues *kv = GET_META( 1, KeyValues );

	kv->SetString( g_Lua->GetString( 2 ), g_Lua->GetString( 3 ) );

	return 0;
}

META_FUNCTION( KeyValues, GetColor )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );
	g_Lua->CheckType( 2, GLua::TYPE_STRING );

	KeyValues *kv = GET_META( 1, KeyValues );

	Color rgb = kv->GetColor( g_Lua->GetString( 2 ) );

	ILuaObject *colorTable = g_Lua->GetNewTable();

	colorTable->SetMember( "r", (float)rgb.r() );
	colorTable->SetMember( "g", (float)rgb.g() );
	colorTable->SetMember( "b", (float)rgb.b() );
	colorTable->SetMember( "a", (float)rgb.a() );

	colorTable->Push();
	colorTable->UnReference();

	return 1;
}

META_FUNCTION( KeyValues, SetColor )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );
	g_Lua->CheckType( 2, GLua::TYPE_STRING );
	g_Lua->CheckType( 3, GLua::TYPE_TABLE );

	KeyValues *kv = GET_META( 1, KeyValues );

	ILuaObject *colorTable = g_Lua->GetObject( 2 );
	
	Color rgb( colorTable->GetMemberInt( "r" ),
				colorTable->GetMemberInt( "g" ),
				colorTable->GetMemberInt( "b" ),
				colorTable->GetMemberInt( "a" ) );

	colorTable->UnReference();

	kv->SetColor( g_Lua->GetString( 2 ), rgb );

	return 0;
}

META_FUNCTION( KeyValues, IsEmpty )
{
	g_Lua->CheckType( 1, GET_META_ID( KeyValues ) );

	KeyValues *kv = GET_META( 1, KeyValues );

	g_Lua->Push( kv->IsEmpty( g_Lua->GetString( 2 ) ) );

	return 1;
}