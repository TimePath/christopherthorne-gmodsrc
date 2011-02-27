{$IFNDEF _WINDEF_}
{$DEFINE _WINDEF_}

(*PASCAL -> C*)

type
	int = longint;
	long = longint;

(*BASETYPES*)

type
	ULONG = longword;
	PULONG = ^ULONG;
	USHORT = word;
	PUSHORT = ^USHORT;
	UCHAR = byte;
	PUCHAR = ^UCHAR;
	
{$DEFINE MAX_PATH := 260}

{$DEFINE NULL := NIL}

{$DEFINE IN}
{$DEFINE OUT}
{$DEFINE OPTIONAL}

{$DEFINE CALLBACK := stdcall}
{$DEFINE WINAPI := stdcall}
{$DEFINE WINAPIV := cdecl}
{$DEFINE APIENTRY := WINAPI}
{$DEFINE APIPRIVATE := stdcall}
{$DEFINE PASCAL := stdcall}

type
	DWORD = longword;
	BOOL = int;
	(*BYTE*)
	(*WORD*)
	FLOAT = real;
	PFLOAT = ^FLOAT;
	PBOOL = ^BOOL;
	LPBOOL = ^BOOL;
	PBYTE = ^BYTE;
	LPBYTE = ^BYTE;
	PINT = ^int;
	LPINT = ^int;
	PWORD = ^WORD;
	LPWORD = ^WORD;
	LPLONG = ^long;
	PDWORD = ^DWORD;
	LPDWORD = ^DWORD;
	LPVOID = POINTER;
	LPCVOID = POINTER;
	
	(*INT*)
	UINT = longword;
	PUINT = ^longword;

{$INCLUDE WinNT.ph}

(* Types use for passing & returning polymorphic values *)

type
	WPARAM = UINT_PTR;
	LPARAM = LONG_PTR;
	LRESULT = LONG_PTR;
		
{$DEFINE MAKEWORD(a, b) := (WORD((BYTE((DWORD_PTR(a)) and $ff)) | (WORD(BYTE((DWORD_PTR(b)) and $ff))) shl 8))}
{$DEFINE MAKELONG(a, b) := (LONG((WORD((DWORD_PTR(a)) and 0xffff)) | (DWORD(WORD((DWORD_PTR(b)) and $ffff))) shl 16))}
{$DEFINE LOWORD(l) := (WORD((DWORD_PTR(l)) and $ffff))}
{$DEFINE HIWORD(l) := (WORD(((DWORD_PTR(l)) shr 16) and $ffff))}
{$DEFINE LOBYTE(w) := (BYTE((DWORD_PTR(w)) and 0xff))}
{$DEFINE HIBYTE(w) := (BYTE(((DWORD_PTR(w)) shr 8) and $ff))}

type
	THWND = THANDLE;
	THHOOK = THANDLE;

type
	ATOM = WORD;

type
	SPHANDLE = ^THANDLE;
	LPHANDLE = ^THANDLE;
	HGLOBAL = THANDLE;
	HLOCAL = THANDLE;
	GLOBALHANDLE = THANDLE;
	LOCALHANDLE = THANDLE;

type
	THKEY = THANDLE;
	PHKEY = ^THKEY;

type
	THACCEL = THANDLE;
	THBITMAP = THANDLE;
	THBRUSH = THANDLE;
	THCOLORSPACE = THANDLE;
	THDC = THANDLE;
	THGLRC = THANDLE; (*OpenGL*)
	THDESK = THANDLE;
	THENHMETAFILE = THANDLE;
	THFONT = THANDLE;
	THICON = THANDLE;
	THMENU = THANDLE;
	THETAFILE = THANDLE;
	THINSTANCE = THANDLE;

type
	THMODULE = THINSTANCE; (* HMODULEs can be used in place of HINSTANCEs *)

type
	THPALETTE = THANDLE;
	THPEN = THANDLE;
	THRGN = THANDLE;
	THSRC = THANDLE;
	THSPRITE = THANDLE;
	THLSURF = THANDLE;
	THSTR = THANDLE;
	THTASK = THANDLE;
	THWINSTA = THANDLE;
	THKL = THANDLE;
	THWINEVENTHOOK = THANDLE;
	THMONITOR = THANDLE;
	THUMPD = THANDLE;

type
	THFILE = int;
	THCURSOR = THICON; (* HICONs & HCURSORs are polymorphic *)
	
	COLORREF = DWORD;
	LPCOLORREF = ^DWORD;
	
{$DEFINE HFILE_ERROR := HFILE(-1)}

type
	tagRECT = record
		left : LONG;
		top : LONG;
		right : LONG;
		bottom : LONG;
	end;
	
	RECT = tagRECT;
	PRECT = ^tagRECT;
	NPRECT = ^tagRECT;
	LPRECT = ^tagRECT;

	LPCRECT = ^RECT;
	
type
	_RECTL = record (* rcl *)
		left : LONG;
		top : LONG;
		right : LONG;
		bottom : LONG;
	end;
	
	RECTL = _RECTL;
	PRECTL = ^_RECTL;
	LPRECTL = ^_RECTL;
	
	LPCRECTL = ^RECTL;
	
type
	tagPOINT = record
		x : LONG;
		y : LONG;
	end;
	
	POINT = tagPOINT;
	PPOINT = ^tagPOINT;
	NPPOINT = ^tagPOINT;
	LPPOINT = ^tagPOINT;
	
type
	_POINTL = record (* ptl *)
		x : LONG;
		y : LONG;
	end;
	
	POINTL = _POINTL;
	PPOINTL = ^_POINTL;
	
type
	tagSIZE = record
		cx : LONG;
		cy : LONG;
	end;
	
	SIZE = tagSize;
	PSIZE = ^tagSize;
	LPSIZE = ^tagSize;
	
	SIZEL = SIZE;
	PSIZEL = ^SIZE;
	LPSIZEL = ^SIZE;
	
type
	tagPOINTS = record
		x : SHORT;
		y : SHORT;
	end;
	
	POINTS = tagPOINTS;
	PPOINTS = ^tagPOINTS;
	LPPOINTS = ^tagPOINTS;

//
//  File System time stamps are represented with the following structure:
//

type
	_FILETIME = record
		dwLowDateTime : DWORD;
		dwHighDateTime : DWORD;
	end;
	
	FILETIME = _FILETIME;
	PFILETIME = ^_FILETIME;
	LPFILETIME = ^_FILETIME;
	
(* mode selections for the device mode function *)
{$DEFINE DM_UPDATE := 1}
{$DEFINE DM_COPY := 2}
{$DEFINE DM_PROMPT := 4}
{$DEFINE DM_MODIFY := 8}

{$DEFINE DM_IN_BUFFER := DM_MODIFY}
{$DEFINE DM_IN_PROMPT := DM_PROMPT}
{$DEFINE DM_OUT_BUFFER := DM_COPY}
{$DEFINE DM_OUT_DEFAULT := DM_UPDATE}

(* device capabilities indices *)
{$DEFINE DC_FIELDS := 1}
{$DEFINE DC_PAPERS := 2}
{$DEFINE DC_PAPERSIZE := 3}
{$DEFINE DC_MINEXTENT := 4}
{$DEFINE DC_MAXEXTENT := 5}
{$DEFINE DC_BINS := 6}
{$DEFINE DC_DUPLEX := 7}
{$DEFINE DC_SIZE := 8}
{$DEFINE DC_EXTRA := 9}
{$DEFINE DC_VERSION := 10}
{$DEFINE DC_DRIVER := 11}
{$DEFINE DC_BINNAMES := 12}
{$DEFINE DC_ENUMRESOLUTIONS := 13}
{$DEFINE DC_FILEDEPENDENCIES := 14}
{$DEFINE DC_TRUETYPE := 15}
{$DEFINE DC_PAPERNAMES := 16}
{$DEFINE DC_ORIENTATION := 17}
{$DEFINE DC_COPIES := 18}

{$ENDIF} (* _WINDEF_ *)