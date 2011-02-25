#include "main.h"
#include "convar.h"
#include "eiface.h"
#include "iserver.h"
#include "tier1/tier1.h"
#include "vfnhook.h"

// platform definitions
#ifdef _WIN32
#define ENGINE_LIB "engine.dll"
#define CBASESERVER_GETNUMCLIENTS_OFFSET 2
#include "csimplescan.h"
#elif defined _LINUX
#define ENGINE_LIB "engine.so"
#define CBASESERVER_GETNUMCLIENTS_OFFSET 3
#include "memutils.h"
#include <dlfcn.h>
#endif

// forward declarations
class CBaseServer;

// engine interfaces
IServer *sv = NULL;
IVEngineServer *engine = NULL;

// globals
bool g_bLoaded = false;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static ConVar sv_visiblecurplayers( "sv_visiblecurplayers", "0", 0, "Overrides the current players reported to prospective clients" );

//---------------------------------------------------------------------------------
// Purpose: called when a client requests server info
//---------------------------------------------------------------------------------
DEFVFUNC_( CBaseServer_GetNumClients_T, int, ( CBaseServer * ) );

int VFUNC CBaseServer_GetNumClients_H( CBaseServer *server )
{
	int clients = CBaseServer_GetNumClients_T( server );

	if ( clients < sv_visiblecurplayers.GetInt() )
		return sv_visiblecurplayers.GetInt();

	return clients;
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is loaded, load the interface we need from the engine
//---------------------------------------------------------------------------------
bool CServerPlugin::Load( CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory )
{
	if ( g_bLoaded )
		return false;

	g_bLoaded = true;

	ConnectTier1Libraries( &interfaceFactory, 1 );

	ConVar_Register();

#ifdef _WIN32
	CSimpleScan engineScan( ENGINE_LIB );

	unsigned long svLocator;

	if ( engineScan.Find( "\x50\xFF\xD2\x6A\x01\xB9", "xxxxxx", (void **)&svLocator ) )
		sv = *(IServer **)(svLocator+6);
#elif defined _LINUX
	void *hEngine = dlopen( ENGINE_LIB, RTLD_LAZY );

	if ( hEngine )
	{
		sv = (IServer *)ResolveSymbol( hEngine, "sv" );

		dlclose( hEngine );
	}
#endif

	CreateInterfaceFn fnEngineFactory = Sys_GetFactory( ENGINE_LIB );

	if ( !fnEngineFactory )
	{
		Msg( "Failed to retrieve CreateInterface function\n" );

		return false;
	}

	engine = (IVEngineServer *)fnEngineFactory( INTERFACEVERSION_VENGINESERVER, NULL );

	if ( !engine )
	{
		Msg( "Failed to retrieve IVEngineServer pointer\n" );

		return false;
	}

	if ( !sv )
	{
		Msg( "Failed to retrieve CBaseServer pointer\n" );

		return false;
	}

	engine->ServerCommand( "sv_master_legacy_mode 1\n" );

	HOOKVFUNC( sv, CBASESERVER_GETNUMCLIENTS_OFFSET, CBaseServer_GetNumClients_T, CBaseServer_GetNumClients_H );

	return true;
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is unloaded (turned off)
//---------------------------------------------------------------------------------
void CServerPlugin::Unload( void )
{
	if ( !g_bLoaded )
		return;

	if ( sv )
		UNHOOKVFUNC( sv, CBASESERVER_GETNUMCLIENTS_OFFSET, CBaseServer_GetNumClients_T );

	if ( engine )
		engine->ServerCommand( "sv_master_legacy_mode 0\n" );

	ConVar_Unregister();

	DisconnectTier1Libraries();
}

//---------------------------------------------------------------------------------
// Purpose: the name of this plugin, returned in "plugin_print" command
//---------------------------------------------------------------------------------
const char *CServerPlugin::GetPluginDescription( void )
{
	return "sv_visiblecurplayers ConVar";
}

// The plugin is a static singleton that is exported as an interface
CServerPlugin g_ServerPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CServerPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_ServerPlugin );