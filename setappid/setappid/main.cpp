#include "main.h"

// Displays a user-friendly error message
void Error( const char *err )
{
	MessageBox( NULL, err, NULL, MB_OK | MB_ICONERROR );
}

// Handles Win32 callbacks
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
		case WM_COMMAND:
		{
			int wParamLo = LOWORD( wParam );
			int wParamHi = HIWORD( wParam );

			if ( wParamLo == HWND_SETID_ID )
			{
				char szAppID[32];
				GetDlgItemTextA( hWnd, HWND_APPID_ID, szAppID, sizeof( szAppID ) );

				if ( g_bInGame )
					g_pSteamUser->TerminateGameConnection( g_unServerIP, g_usServerPort );
				else
					g_bInGame = true;

				unsigned char pBlob[2048];
				unsigned int pBlobSize = g_pSteamUser->InitiateGameConnection( pBlob, sizeof( pBlob ), CSteamID( (uint64)0 ), CGameID( atoi( szAppID ) ), g_unServerIP, g_usServerPort, false );
				
				g_pClientUtils->SetAppIDForCurrentPipe( atoi( szAppID ), true );

				EnableWindow( g_hWnd_AppID, false );
				EnableWindow( g_hWnd_SetID, false );

				while ( GetAsyncKeyState( VK_ESCAPE ) == 0 )
					Sleep( 10 );

				EnableWindow( g_hWnd_AppID, true );
				EnableWindow( g_hWnd_SetID, true );
			}

			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);

			return 0;
		}
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

// Loads Steam client interfaces
bool STEAM_Load()
{
	CreateInterfaceFn fnApiInterface = g_SteamAPILoader.Load();

	if ( !fnApiInterface )
		return false;

	// Versioned interfaces

	if ( !( g_pSteamClient = (ISteamClient008 *)fnApiInterface( STEAMCLIENT_INTERFACE_VERSION_008, NULL ) ) )
		return false;

	if ( !( g_hSteamPipe = g_pSteamClient->CreateSteamPipe() ) )
		return false;

	if ( !( g_hSteamUser = g_pSteamClient->ConnectToGlobalUser( g_hSteamPipe ) ) )
		return false;

	if ( !( g_pSteamUser = (ISteamUser009 *)g_pSteamClient->GetISteamUser( g_hSteamUser, g_hSteamPipe, STEAMUSER_INTERFACE_VERSION_009 ) ) )
		return false;

	// Latest interfaces

	if ( !( g_pClientEngine = (IClientEngine *)fnApiInterface( CLIENTENGINE_INTERFACE_VERSION, NULL ) ) )
		return false;

	if ( !( g_pClientUtils = (IClientUtils *)g_pClientEngine->GetIClientUtils( g_hSteamPipe, CLIENTUTILS_INTERFACE_VERSION ) ) )
		return false;

	return true;
}

void STEAM_Unload()
{
	if ( g_bInGame )
		g_pSteamUser->TerminateGameConnection( g_unServerIP, g_usServerPort );
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{	
	if ( !STEAM_Load() )
	{
		Error( "STEAM_Load() failed" );
		
		return 0;
	}

	WNDCLASSEX wc;
	memset( &wc, 0, sizeof( WNDCLASSEX ) );

	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = SETAPPID_CLASS;

	RegisterClassEx( &wc );

	g_hWnd = CreateWindowA( SETAPPID_CLASS, "AppID", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 0, 55, NULL, NULL, hInstance, NULL );
	g_hWnd_AppID = CreateWindowA( "EDIT", "0", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_NUMBER, 13, 6, 50, 14, g_hWnd, (HMENU)HWND_APPID_ID, hInstance, NULL );
	g_hWnd_SetID = CreateWindowA( "BUTTON", "SET", WS_CHILD | WS_VISIBLE, 73, 6, 40, 14, g_hWnd, (HMENU)HWND_SETID_ID, hInstance, NULL );
	
	HFONT arial = CreateFont( 14, 0, 0, 0, 300, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial" );
	
	SendMessageA( g_hWnd_AppID, WM_SETFONT, (WPARAM)arial, true );
	SendMessageA( g_hWnd_SetID, WM_SETFONT, (WPARAM)arial, true );

	ShowWindow( g_hWnd, nShowCmd );

	MSG msg;

	while ( true )
	{
		while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if ( msg.message == WM_QUIT )
			break;
	}

	DeleteObject( arial );

	UnregisterClass( SETAPPID_CLASS, hInstance );

	STEAM_Unload();

	return 0;
}