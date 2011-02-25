#include "gl_imaterialvar.h"

META_ID( IMaterialVar, 1 );

META_FUNCTION( IMaterialVar, GetName )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	g_Lua->Push( pVar->GetName() );

	return 1;
}

META_FUNCTION( IMaterialVar, GetTextureValue )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	PUSH_META_LOOKUP( pVar->GetTextureValue(), "ITexture", GLua::TYPE_TEXTURE );

	return 1;
}

META_FUNCTION( IMaterialVar, SetTextureValue )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );
	g_Lua->CheckType( 2, GLua::TYPE_TEXTURE );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );
	ITexture *pTexture = GET_META( 2, ITexture );

	pVar->SetTextureValue( pTexture );

	return 0;
}

META_FUNCTION( IMaterialVar, GetFloatValue )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	g_Lua->Push( pVar->GetFloatValue() );

	return 1;
}

META_FUNCTION( IMaterialVar, SetFloatValue )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );
	g_Lua->CheckType( 2, GLua::TYPE_NUMBER );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	pVar->SetFloatValue( g_Lua->GetNumber( 2 ) );

	return 0;
}

META_FUNCTION( IMaterialVar, GetIntValue )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	g_Lua->PushLong( pVar->GetIntValue() );

	return 1;
}

META_FUNCTION( IMaterialVar, SetIntValue )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );
	g_Lua->CheckType( 2, GLua::TYPE_NUMBER );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	pVar->SetIntValue( g_Lua->GetInteger( 2 ) );

	return 0;
}

META_FUNCTION( IMaterialVar, GetStringValue )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	g_Lua->Push( pVar->GetStringValue() );

	return 1;
}

META_FUNCTION( IMaterialVar, SetStringValue )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );
	g_Lua->CheckType( 2, GLua::TYPE_STRING );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	pVar->SetStringValue( g_Lua->GetString( 2 ) );

	return 1;
}

META_FUNCTION( IMaterialVar, GetMaterialValue )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	PUSH_META_LOOKUP( pVar->GetMaterialValue(), "IMaterial", GLua::TYPE_MATERIAL );

	return 1;
}

META_FUNCTION( IMaterialVar, SetMaterialValue )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );
	g_Lua->CheckType( 2, GLua::TYPE_MATERIAL );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );
	IMaterial *pMaterial = GET_META( 2, IMaterial );

	pVar->SetMaterialValue( pMaterial );

	return 0;
}

META_FUNCTION( IMaterialVar, GetVecValue )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	ILuaObject *vecTable = g_Lua->GetNewTable();


	if ( g_Lua->GetType( 2 ) == GLua::TYPE_NUMBER )
	{
		Vector vec3;

		pVar->GetVecValue( vec3.Base(), g_Lua->GetInteger( 2 ) );

		vecTable->SetMember( 1, vec3.x );
		vecTable->SetMember( 2, vec3.y );
		vecTable->SetMember( 3, vec3.z );
	}
	else
	{
		Vector4D vec4( pVar->GetVecValue() );
	
		vecTable->SetMember( 1, vec4.x );
		vecTable->SetMember( 2, vec4.y );
		vecTable->SetMember( 3, vec4.z );
		vecTable->SetMember( 4, vec4.w );
	}

	vecTable->Push();
	vecTable->UnReference();

	return 1;
}

META_FUNCTION( IMaterialVar, SetVecValue )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );
	g_Lua->CheckType( 2, GLua::TYPE_TABLE );
	g_Lua->CheckType( 3, GLua::TYPE_NUMBER );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	ILuaObject *vecTable = g_Lua->GetObject( 2 );

	float v[4];
	int vecSize = min( g_Lua->GetInteger( 3 ), 4 );

	for ( int i = 0; i < vecSize; i++ )
	{
		ILuaObject *memberObj = vecTable->GetMember( (float)i + 1 );

		if ( memberObj )
		{
			v[i] = memberObj->GetFloat();

			memberObj->UnReference();
		}
	}

	vecTable->UnReference();

	pVar->SetVecValue( v, vecSize );

	return 0;
}

META_FUNCTION( IMaterialVar, GetMatrixValue )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	PUSH_META_LOOKUP( (void *)&pVar->GetMatrixValue(), "VMatrix", GLua::TYPE_MATRIX );

	return 1;
}

META_FUNCTION( IMaterialVar, SetMatrixValue )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );
	g_Lua->CheckType( 2, GLua::TYPE_MATRIX );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	VMatrix *pMatrix = GET_META( 2, VMatrix );

	pVar->SetMatrixValue( *pMatrix );

	return 0;
}

META_FUNCTION( IMaterialVar, SetVecComponentValue )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );
	g_Lua->CheckType( 2, GLua::TYPE_NUMBER );
	g_Lua->CheckType( 3, GLua::TYPE_NUMBER );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	pVar->SetVecComponentValue( g_Lua->GetNumber( 2 ), g_Lua->GetInteger( 3 ) );

	return 0;
}

META_FUNCTION( IMaterialVar, VectorSize )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	g_Lua->PushLong( pVar->VectorSize() );

	return 1;
}

META_FUNCTION( IMaterialVar, IsDefined )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	g_Lua->Push( pVar->IsDefined() );

	return 1;
}

META_FUNCTION( IMaterialVar, SetUndefined )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	pVar->SetUndefined();

	return 0;
}

META_FUNCTION( IMaterialVar, GetOwningMaterial )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	PUSH_META_LOOKUP( pVar->GetOwningMaterial(), "IMaterial", GLua::TYPE_MATERIAL );

	return 1;
}

META_FUNCTION( IMaterialVar, GetType )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	g_Lua->PushLong( pVar->GetType() );

	return 1;
}

META_FUNCTION( IMaterialVar, IsTexture )
{
	g_Lua->CheckType( 1, GET_META_ID( IMaterialVar ) );

	IMaterialVar *pVar = GET_META( 1, IMaterialVar );

	return 1;
}