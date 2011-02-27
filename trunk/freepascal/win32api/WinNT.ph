{$IFNDEF _WINNT_}
{$DEFINE _WINNT_}

{$INCLUDE BaseTsd.ph}

//
// Void
//

type
	PVOID = POINTER;

//
// Basics
//

type
	SHORT = smallint;

//
// ANSI (Multi-byte Character) types
//

type
	(*PCHAR*)
	LPCH = ^CHAR;
	PCH = ^CHAR;

	LPCCH = ^CHAR;
	PCCH = ^CHAR;

type
	NPSTR = ^CHAR;
	LPSTR = ^CHAR;
	PSTR = ^CHAR;
	
	PZPSTR = ^CHAR;
	
	PCZPSTR = ^CHAR;
	
	LPCSTR = ^CHAR;
	PCSTR = ^CHAR;
	
	PZPCSTR = ^PCSTR;
	
type
	PZZSTR = ^CHAR;
	PCZZZSTR = ^CHAR;
	
type
	PNZCH = ^CHAR;
	PCNZCH = ^CHAR;

type
	TCHAR = char;
	PTCHAR = ^char;
	
	TBYTE = byte;
	PTBYTE = ^byte;

(*
TODO:
	typedef LPCH LPTCH, PTCH;
	typedef LPCCH LPCTCH, PCTCH;
	typedef LPSTR PTSTR, LPTSTR, PUTSTR, LPUTSTR;
	typedef LPCSTR PCTSTR, LPCTSTR, PCUTSTR, LPCUTSTR;
	typedef PZZSTR PZZTSTR, PUZZTSTR;
	typedef PCZZSTR PCZZTSTR, PCUZZTSTR;
	typedef PNZCH PNZTCH, PUNZTCH;
	typedef PCNZCH PCNZTCH, PCUNZTCH;
*)

type
	PSHORT = ^SHORT;
	PLONG = ^LONG;
	
{$DEFINE ALL_PROCESSOR_GROUPS := $FFFF}

//
// Structure to represent a system wide processor number. It contains a
// group number and relative processor number within the group.
//

type
	_PROCESSOR_NUMBER = record
		Group : WORD;
		Number : BYTE;
		Reserved : BYTE;
	end;
	
	PROCESSOR_NUMBER = _PROCESSOR_NUMBER;
	PPROCESSOR_NUMBER = ^_PROCESSOR_NUMBER;

//
// Structure to represent a group-specific affinity, such as that of a
// thread.  Specifies the group number and the affinity within that group.
//

type
	_GROUP_AFFINITY = record
		Mask : KAFFINITY;
		Group : WORD;
		Reserved : array[0..3] of WORD;
	end;

	GROUP_AFFINITY = _GROUP_AFFINITY;
	PGROUP_AFFINITY = ^_GROUP_AFFINITY;

//
// Handle to an Object
//

type
	THANDLE = POINTER;

type
	PHANDLE = ^THANDLE;
	
//
// Flag (bit) fields
//

type
	FCHAR = BYTE;
	FSHORT = WORD;
	FLONG = DWORD;
	
// Component Object Model defines, and macros

(*TODO*)

{$ENDIF} (* _WINNT_ *)