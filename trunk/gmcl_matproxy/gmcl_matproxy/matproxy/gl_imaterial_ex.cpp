#include "gl_imaterial_ex.h"
#include "gl_imaterialvar.h"
#include "main.h"

META_FUNCTION( IMaterial, FindVar )
{
	g_Lua->CheckType( 1, GLua::TYPE_MATERIAL );
	g_Lua->CheckType( 2, GLua::TYPE_STRING );

	IMaterial *pMaterial = GET_META( 1, IMaterial );

	bool found;
	IMaterialVar *pVar = pMaterial->FindVar( g_Lua->GetString( 2 ), &found, false );

	if ( found )
	{
		PUSH_META( pVar, IMaterialVar );
	}
	else
	{
		g_Lua->PushNil();
	}

	return 1;
}