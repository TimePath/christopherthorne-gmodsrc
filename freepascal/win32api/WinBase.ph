{$IFNDEF _WINBASE_}
{$DEFINE _WINBASE_}

{$DEFINE WINBASEAPI := external 'kernel32.dll'}

function GetModuleHandleA(
	lpModuleName : LPCSTR
) : THMODULE;WINAPI;WINBASEAPI;

{$DEFINE GetModuleHandle := GetModuleHandleA}

{$ENDIF} // _WINBASE_