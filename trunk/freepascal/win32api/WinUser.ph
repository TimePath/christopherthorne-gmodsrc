{$IFNDEF _WINUSER_}
{$DEFINE _WINUSER_}

{$DEFINE WINUSERAPI := external 'user32.dll'}

type
	HDWP = THANDLE;
	
type
	(*MENUTEMPLATEA*)
	(*MENUTEMPLATEW*)
	LPMENUTEMPLATEA = PVOID;

type
	TWNDPROC = function(hWnd : THWND; message : UINT; wParam : WPARAM; lParam : LPARAM) : LRESULT;CALLBACK;

(*Unnecessary shit*)

{$DEFINE IS_INTRESOURCE(_r) (((ULONG_PTR(_r)) shr 16) = 0)}
{$DEFINE MAKEINTRESOURCE(i) := LPSTR(ULONG_PTR(WORD(i)))}

(*
 * ShowWindow() Commands
 *)

{$DEFINE SW_SHOWDEFAULT := 10}

(*
 * Old ShowWindow() Commands
 *)
 
(*
 * Identifiers for the WM_SHOWWINDOW message
 *)
 
(*
 * AnimateWindow() Commands
 *)

(*
 * WM_KEYUP/DOWN/CHAR HIWORD(lParam) flags
 *)

(*
 * Virtual Keys, Standard Set
 *)

(*
 * SetWindowsHook() codes
 *)

(*
 * Hook Codes
 *)

(*
 * CBT Hook Codes
 *)

(*
 * HCBT_CREATEWND parameters pointed to by lParam
 *)

(*
 * HCBT_CREATEWND parameters pointed to by lParam
 *)

(*
 * HCBT_ACTIVATE structure pointed to by lParam
 *)

(*More shit*)

type
	tagWNDCLASSEX = record
		cbSize : UINT;
		style : UINT;
		lpfnWndProc : TWNDPROC;
		cbClsExtra : int;
		cbWndExtra : int;
		hInstance : THINSTANCE;
		hIcon : THICON;
		hCursor : THCURSOR;
		hbrBackground : THBRUSH;
		lpszMenuName : LPCSTR;
		lpszClassName : LPCSTR;
		hIconSm : THICON;
	end;
	
	WNDCLASSEX = tagWNDCLASSEX;
	PWNDCLASSEX = ^tagWNDCLASSEX;
	NPWNDCLASSEX = ^tagWNDCLASSEX;
	LPWNDCLASSEX = ^tagWNDCLASSEX;
	
type
	tagWNDCLASS = record
	end;
	
	WNDCLASS = tagWNDCLASS;
	PWNDCLASS = ^tagWNDCLASS;
	NPWNDCLASS = ^tagWNDCLASS;
	LPWNDCLASS = ^tagWNDCLASS;

type
	tagMSG = record
		hwnd : THWND;
		message : UINT;
		wParam : WPARAM;
		lParam : LPARAM;
		time : DWORD;
		pt : POINT;
	end;
	
	MSG = tagMSG;
	PMSG = ^tagMSG;
	NPMSG = ^tagMSG;
	LPMSG = ^tagMSG;

{$DEFINE MAKEWPARAM(l, h) := (WPARAM(DWORD(MAKELONG(l, h))))}
{$DEFINE MAKELPARAM(l, h) := (LPARAM(DWORD(MAKELONG(l, h))))}
{$DEFINE MAKELRESULT(l, h) := (LRESULT(DWORD(MAKELONG(l, h))))}

(*
 * Window field offsets for GetWindowLong()
 *)

 (*
 * Class field offsets for GetClassLong()
 *)

(*
 * Window Messages
 *)

{$DEFINE WM_DESTROY := $0002}
{$DEFINE WM_CLOSE := $0010}

(*...*)

(*
 * Window Styles
 *)

{$DEFINE WS_OVERLAPPED := $00000000}
{$DEFINE WS_POPUP := $80000000}
{$DEFINE WS_CHILD := $40000000}
{$DEFINE WS_MINIMIZE := $20000000}
{$DEFINE WS_VISIBLE := $10000000}
{$DEFINE WS_DISABLED := $08000000}
{$DEFINE WS_CLIPSIBLINGS := $04000000}
{$DEFINE WS_CLIPCHILDREN := $02000000}
{$DEFINE WS_MAXIMIZE := $01000000}
{$DEFINE WS_CAPTION := $00C00000}
{$DEFINE WS_BORDER := $00800000}
{$DEFINE WS_DLGFRAME := $00400000}
{$DEFINE WS_VSCROLL := $00200000}
{$DEFINE WS_HSCROLL := $00100000}
{$DEFINE WS_SYSMENU := $00080000}
{$DEFINE WS_THICKFRAME := $00040000}
{$DEFINE WS_GROUP := $00020000}
{$DEFINE WS_TABSTOP := $00010000}

{$DEFINE WS_MINIMIZEBOX := $00020000}
{$DEFINE WS_MAXIMIZEBOX := $00010000}
 
{$DEFINE WS_TILED := WS_OVERLAPPED}
{$DEFINE WS_ICONIC := WS_MINIMIZE}
{$DEFINE WS_SIZEBOX := WS_THICKFRAME}
{$DEFINE WS_TILEDWINDOW := WS_OVERLAPPEDWINDOW}

(*
 * Common Window Styles
 *)
 
{$DEFINE WS_OVERLAPPEDWINDOW := WS_OVERLAPPED or
								WS_CAPTION or
								WS_SYSMENU or
								WS_THICKFRAME or
								WS_MINIMIZEBOX or
								WS_MAXIMIZEBOX}

(*
 * Extended Window Styles
 *)

{$DEFINE WS_EX_CLIENTEDGE := $00000200}

(*
 * Class styles
 *)
{$DEFINE CS_VREDRAW := $0001}
{$DEFINE CS_HREDRAW := $0002}

(*
 * Standard Cursor IDs
 *)

{$DEFINE IDC_ARROW := MAKEINTRESOURCE(32512)}

(*
 * Standard Icon IDs
 *)

{$DEFINE IDI_APPLICATION := MAKEINTRESOURCE(32512)}

(*
 * MessageBox() Flags
 *)

{$DEFINE MB_OK := $0000000}

{$DEFINE MB_ICONHAND := $00000010}
{$DEFINE MB_ICONQUESTION := $00000020}
{$DEFINE MB_ICONEXCLAMATION := $00000030}
{$DEFINE MB_ICONASTERISK := $00000040}

function MessageBoxA(
	hWnd : THWND;
	lpText : LPCSTR;
	lpCaption : LPCSTR;
	uType : UINT
) : int;WINAPI;WINUSERAPI;

{$DEFINE MessageBox := MessageBoxA}

function GetMessageA(
	lpMsg : LPMSG;
	hWnd : THWND;
	wMsgFilterMin : UINT;
	wMsgFilterMax : UINT
) : BOOL;WINAPI;WINUSERAPI;

{$DEFINE GetMessage := GetMessageA}

function TranslateMessage(
	lpMsg : LPMSG
) : BOOL;WINAPI;WINUSERAPI;

function DispatchMessageA(
	lpMsg : LPMSG
) : BOOL;WINAPI;WINUSERAPI;

{$DEFINE DispatchMessage := DispatchMessageA}

function RegisterClassA(
	lpWndClass : LPWNDCLASS
) : ATOM;WINAPI;WINUSERAPI;

{$DEFINE RegisterClass := RegisterClassA}

function UnregisterClassA(
	lpClassName : LPCSTR;
	hInstance : THINSTANCE
) : BOOL;WINAPI;WINUSERAPI;

{$DEFINE UnregisterClass := UnregisterClassA}

function RegisterClassExA(
	lpwcx : LPWNDCLASSEX
) : ATOM;WINAPI;WINUSERAPI;

{$DEFINE RegisterClassEx := RegisterClassExA}

{$DEFINE CW_USEDEFAULT := word($80000000)}

(*
 * Special value for CreateWindow, et al.
 *)
{$DEFINE HWND_DESKTOP := HWND(0)}

function CreateWindowExA(
    dwExStyle : DWORD;
    lpClassName : LPCSTR;
    lpWindowName : LPCSTR;
    dwStyle : DWORD;
    X : int;
    Y : int;
    nWidth : int;
    nHeight : int;
    hWndParent : THWND;
    hMenu : THMENU;
    hInstance : THINSTANCE;
    lpParam : LPVOID
) : THWND;WINAPI;WINUSERAPI;

{$DEFINE CreateWindowEx := CreateWindowExA}

{$DEFINE CreateWindowA(lpClassName, lpWindowName, dwStyle, x, y,
nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam) :=
CreateWindowExA(0, lpClassName, lpWindowName, dwStyle, x, y,
nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)}

{$DEFINE CreateWindow := CreateWindowA}

function DestroyWindow(
	hWnd : THWND
) : BOOL;WINAPI;WINUSERAPI;

function ShowWindow(
	hWnd : THWND;
	nCmdShow : int
) : BOOL;WINAPI;WINUSERAPI;

function UpdateWindow(
	hWnd : THWND
) : BOOL;WINAPI;WINUSERAPI;

function DefWindowProcA(
	hWnd : THWND;
	message : UINT;
	wParam : WPARAM;
	lParam : LPARAM
) : LRESULT;WINAPI;WINUSERAPI;

{$DEFINE DefWindowProc := DefWindowProcA}

procedure PostQuitMessage(
	nExitCode : int
);WINAPI;WINUSERAPI;

(*
 * Color Types
 *)

{$DEFINE COLOR_WINDOW := 5}
{$DEFINE COLOR_WINDOWFRAME := 6}

function LoadIconA(
	hInstance : THINSTANCE;
	lpIconName : LPCSTR
) : THICON;WINAPI;WINUSERAPI;

{$DEFINE LoadIcon := LoadIconA}

function LoadCursorA(
	hInstance : THINSTANCE;
	lpCursorName : LPCSTR
) : THCURSOR;WINAPI;WINUSERAPI;

{$DEFINE LoadCursor := LoadCursorA}

{$ENDIF} (* _WINUSER_ *)