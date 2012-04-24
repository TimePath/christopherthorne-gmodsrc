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

	// If the hook returns anything other than nil or true then
	// do not add to download queue.
	if ( returnObject->isNil() || returnObject->GetBool() )
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
	// Store Lua interface
	gLua = Lua();

	// Setup engine.dll scanner
	CByteScanner engineScanner( "engine.dll" );

	// Scan for CL_QueueDownload
	CL_QueueDownload_T = (CL_QueueDownload_t)engineScanner.FindCodePattern(
		"\x55" // push ebp
		"\x8B\xEC" // mov ebp, esp
		"\xF7\x05\x44\x0C\xC4\x77\x00\x10\x00\x00" // test dword ptr ds:[xxxxxxxx], 1000
		"\x74\x18" // je short engine.xxxxxxxx
		"\x8B\x4D\x08", // mov ecx, dword ptr ss:[ebp+8]
		"xxxxx????xxxxx?xxx" ); // mask

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