#include "GMLuaModule.h"

#include "bytescanner/cbytescanner.h"

#include "simpledetour/csimpledetour.h"

// Entry points
GMOD_MODULE( Open, Close );

// Global Lua interface
ILuaInterface *gLua = NULL;

// CL_QueueDownload format
typedef void ( *CL_QueueDownload_t )( const char *pszFileName );

// CL_QueueDownload trampoline
CL_QueueDownload_t CL_QueueDownload_T = NULL;

// CL_QueueDownload hook
void CL_QueueDownload_H( const char *pszFileName )
{
	// hook table
	ILuaObject *hookTable = gLua->GetGlobal( "hook" );
	
	// hook.Call function
	ILuaObject *hookCallFunction = hookTable->GetMember( "Call" );
	hookCallFunction->Push();

	// Argument 1 (Hook name)
	gLua->Push( "ShouldDownload" );

	// Argument 2 (Gamemode)
	gLua->PushNil();

	// Argument 3 (File name)
	gLua->Push( pszFileName );

	// hook.Call( "ShouldDownload", nil, pszFileName );
	gLua->Call( 3, 1 );

	// Returned value
	ILuaObject *returnObject = gLua->GetReturn( 0 );
	
	bool bDownload;

	// If the hook returns anything other than nil or true then
	// do not add to download queue.
	if ( returnObject->isNil() || returnObject->GetBool() )
		bDownload = true;
	else
		bDownload = false;

	if ( bDownload )
		CL_QueueDownload_T( pszFileName );

	// Cleanup
	returnObject->UnReference();
	hookCallFunction->UnReference();
	hookTable->UnReference();
}

// CL_QueueDownload detour
SETUP_SIMPLE_DETOUR( CL_QueueDownload_Detour, CL_QueueDownload_T, CL_QueueDownload_H );

int Open( lua_State *L )
{
	// Setup engine.dll scanner
	CByteScanner engineScanner( "engine.dll" );

	// Store Lua interface
	gLua = Lua();

	// Scan for CL_QueueDownload
	CL_QueueDownload_T = (CL_QueueDownload_t)engineScanner.FindCodePattern( "\xF7\x05\x54\xCB\xD2\x0D\x00\x10\x00\x00\x74\x16\x8B\x4C\x24\x04", "xx????xxxxx?xxxx" );

	if ( CL_QueueDownload_T )
	{
		// Attach CL_QueueDownload hook
		CL_QueueDownload_Detour.Attach();
	}
	else
	{
		// Notify client of failure
		gLua->Error( "[gmcl_downloadfilter] Couldn't find CL_QueueDownload" );
	}

	return 0;
}

int Close( lua_State *L )
{
	if ( CL_QueueDownload_T )
	{
		// Detach CL_QueueDownload hook
		CL_QueueDownload_Detour.Detach();
	}

	return 0;
}