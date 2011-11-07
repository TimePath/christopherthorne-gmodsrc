#ifndef WINDOW_WIN32_H
#define WINDOW_WIN32_H

#include <windows.h>

class CGameWindow
{
public:
	CGameWindow( HINSTANCE hInstance, WNDPROC WndProc, const char *title, int width, int height, bool windowed );

	HWND GetHandle() { return m_Window; }
private:
	bool m_Showing;
	WNDCLASSEXA m_WindowClass;
	HWND m_Window;
};

#endif // WINDOW_WIN32_H