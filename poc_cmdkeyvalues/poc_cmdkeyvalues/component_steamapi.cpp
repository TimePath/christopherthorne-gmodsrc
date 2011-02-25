// Components
#include "component_steamapi.h"

// SDK headers
#include "dbg.h"

// OSW headers
#include "steamapi.h"
#include "steamclient.h"

// Globals
HSteamPipe g_hSteamPipe;
HSteamUser g_hSteamUser;

ISteamClient008 *g_pSteamClient = NULL;
ISteamUser009 *g_pSteamUser = NULL;

CSteamAPILoader apiSteam;

REGISTER_COMPONENT_INIT( SteamAPI )
{
	CreateInterfaceFn fnApiInterface = apiSteam.Load();

	if ( !fnApiInterface )
		return false;

	if ( !( g_pSteamClient = (ISteamClient008 *)fnApiInterface( STEAMCLIENT_INTERFACE_VERSION_008, NULL ) ) )
		return false;

	if ( !( g_hSteamPipe = g_pSteamClient->CreateSteamPipe() ) )
		return false;

	if ( !( g_hSteamUser = g_pSteamClient->ConnectToGlobalUser( g_hSteamPipe ) ) )
		return false;

	if ( !( g_pSteamUser = (ISteamUser009 *)g_pSteamClient->GetISteamUser( g_hSteamUser, g_hSteamPipe, STEAMUSER_INTERFACE_VERSION_009 ) ) )
		return false;

	return true;
}

REGISTER_COMPONENT_SHUTDOWN( SteamAPI )
{
	if ( g_hSteamPipe && g_pSteamClient )
		g_pSteamClient->ReleaseSteamPipe( g_hSteamPipe );
}