program win32api;

{$INCLUDE windows.ph}

const
	g_ClassName : PCHAR = 'FreePascalWindow';
	g_WindowName : PCHAR = 'Pascal Window';

var
	hWnd : THWND;
	hInstance : THINSTANCE;
	winmsg : MSG;
	wc : WNDCLASSEX;

function WndProc(hWnd : THWND; message : UINT; wParam : WPARAM; lParam : LPARAM) : LRESULT;CALLBACK;
begin
	case message of
		WM_CLOSE:
		begin
			DestroyWindow(hWnd);
		end;
		WM_DESTROY:
		begin
			PostQuitMessage(0);
		end;
		else
		begin
			exit(DefWindowProcA(hWnd, message, wParam, lParam));
		end;
	end;
	
	exit(0);
end;

begin
	FreeConsole(); (*Hide console - still flashes quickly, find alternative?*)

	hInstance := GetModuleHandleA(NIL); (*Get process instance handle*)

	wc.cbSize := sizeof(WNDCLASSEX);
	wc.style := 0;
	wc.lpfnWndProc := @WndProc;
	wc.cbClsExtra := 0;
	wc.cbWndExtra := 0;
	wc.hInstance := hInstance;
	wc.hIcon := LoadIconA(NIL, IDI_APPLICATION);
	wc.hCursor := LoadCursorA(NIL, IDC_ARROW);
	wc.hbrBackground := THBRUSH(COLOR_WINDOW+1);
	wc.lpszMenuName := NIL;
	wc.lpszClassName := g_ClassName;
	wc.hIconSm := LoadIconA(NIL, IDI_APPLICATION);

	if RegisterClassExA(@wc) = 0 then
		MessageBoxA(NIL, 'Class registration failed', 'Error', MB_OK or MB_ICONHAND);

	hWnd := CreateWindowExA(
		WS_EX_CLIENTEDGE,
		g_ClassName,
		g_WindowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
		NIL, NIL, hInstance, NIL);

	if hWnd = NIL then
		MessageBoxA(NIL, 'Window creation failed', 'Error', MB_OK or MB_ICONHAND);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	while GetMessage(@winmsg, NIL, 0, 0) > 0 do (*Enter message loop*)
	begin
		TranslateMessage(@winmsg);
		DispatchMessageA(@winmsg);
	end;
end.