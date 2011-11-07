#include "window_win32.h"

#define CLASS_NAME "GameWindowClass"

CGameWindow::CGameWindow( HINSTANCE hInstance, WNDPROC WndProc, const char *title, int width, int height, bool windowed )
{
	m_Showing = false;

	m_WindowClass.cbSize = sizeof( m_WindowClass );
	m_WindowClass.style = CS_VREDRAW | CS_HREDRAW;
	m_WindowClass.lpfnWndProc = WndProc;
	m_WindowClass.cbClsExtra = 0;
	m_WindowClass.cbWndExtra = 0;
	m_WindowClass.hInstance = hInstance;
	m_WindowClass.hIcon = LoadIconA( NULL, IDI_APPLICATION );
	m_WindowClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	m_WindowClass.hbrBackground = windowed ? (HBRUSH)COLOR_WINDOW : (HBRUSH)0;
	m_WindowClass.lpszMenuName = NULL;
	m_WindowClass.lpszClassName = CLASS_NAME;
	m_WindowClass.hIconSm = NULL;

	if ( RegisterClassExA( &m_WindowClass ) == 0 )
		return;

	m_Window = CreateWindowA( CLASS_NAME, title,
							  windowed ? WS_OVERLAPPEDWINDOW : WS_EX_TOPMOST | WS_POPUP,
							  windowed ? CW_USEDEFAULT : 0,
							  windowed ? CW_USEDEFAULT : 0,
							  width, height,
							  NULL,
							  NULL,
							  hInstance,
							  NULL );

	if ( !m_Window )
		return;

	ShowWindow( m_Window, SW_SHOWDEFAULT );

	m_Showing = true;
}