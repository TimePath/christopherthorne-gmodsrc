//////////////////////////////
/////////// WIN32 ////////////
//////////////////////////////

// Dependencies
#include <Windows.h>
// Class name
#define SETAPPID_CLASS "SetAppID001"
// Window IDs
#define HWND_APPID_ID 9000
#define HWND_SETID_ID 9001
// Window handles
HWND g_hWnd;
HWND g_hWnd_AppID;
HWND g_hWnd_SetID;

//////////////////////////////
/////////// STEAM ////////////
//////////////////////////////

// Dependencies
#define STEAMWORKS_CLIENT_INTERFACES
#include "osw/Steamworks.h"
#include "osw/ISteamUser009.h"
// API loader
CSteamAPILoader g_SteamAPILoader;
// Client handles
HSteamPipe g_hSteamPipe;
HSteamUser g_hSteamUser;
// Versioned interfaces
ISteamClient008 *g_pSteamClient = NULL;
ISteamUser009 *g_pSteamUser = NULL;
// Latest interfaces
IClientEngine *g_pClientEngine = NULL;
IClientUtils *g_pClientUtils = NULL;
// Game Info
bool g_bInGame = false;
unsigned int g_unServerIP = htonl( inet_addr( "127.0.0.1" ) );
unsigned short g_usServerPort = 27015;