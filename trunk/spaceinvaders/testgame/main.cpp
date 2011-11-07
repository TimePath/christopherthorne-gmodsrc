#include <windows.h>
#include "win32_config.h"

#include "testgame.h"
#include "debug.h"
#include "../renderer/render_interface.h"
#include "../filesystem/filesystem_interface.h"

LRESULT CALLBACK WndProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch ( Msg )
	{
	case WM_DESTROY:
		{
			PostQuitMessage( 0 );

			break;
		}
	default:
		{
			return DefWindowProc( hWnd, Msg, wParam, lParam );
		}
	}

	return 0;
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	WNDCLASSEX wc;
	memset( &wc, 0, sizeof( wc ) );

	wc.cbSize = sizeof( wc );
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.lpszClassName = CLASS_NAME;
	
	if ( RegisterClassEx( &wc ) == 0 )
	{
		Error( "RegisterClassEx failed" );

		return 0;
	}

	HWND hWnd = CreateWindowA( CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL );

	if ( !hWnd )
	{
		Error( "hWnd == NULL" );

		return 0;
	}

	if ( !Render()->Init( hWnd, true, WINDOW_WIDTH, WINDOW_HEIGHT ) )
	{
		Error( "Failed to initialize renderer" );

		return 0;
	}

	if ( TestGame::Initialize() )
	{
		IFileHandle *file = FileSystem()->Open( "blah.txt" );
		file->Write( "a", 1 );
		file->Close();

		ShowWindow( hWnd, nCmdShow );

		bool quit = false;

		while ( !quit )
		{
			MSG msg;

			while ( PeekMessageA( &msg, NULL, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );

				if ( msg.message == WM_QUIT )
				{
					quit = true;

					break;
				}
			}

			Render()->BeginFrame();

			TestGame::Frame();

			Render()->EndFrame();
		}

		TestGame::Shutdown();
	}
	else
	{
		Error( "Failed to initialize game" );
	}

	Render()->Shutdown();

	return 0;
}