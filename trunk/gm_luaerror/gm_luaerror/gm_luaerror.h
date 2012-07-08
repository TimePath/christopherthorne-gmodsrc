#ifndef GM_LUAERROR_H
#define GM_LUAERROR_H

// Headers

#include "GMLuaModule.h"
#include "cbytescanner.h"
#include "csimpledetour.h"

#include <windows.h>
#include <interface.h>
#include <utlvector.h>

// "lua_shared" library

#define LUA_SHARED_LIBRARY "lua_shared.dll"

// CLuaInterface hooks

class CLuaInterface_Hooks
{
public:
	void ErrorFromLua_H( const char *, ... );

	static void ( CLuaInterface_Hooks::* ErrorFromLua_T )( const char *, ... );
};

void ( CLuaInterface_Hooks::* CLuaInterface_Hooks::ErrorFromLua_T )( const char *, ... ) = NULL;

// CLuaInterface::ErrorFromLua signature

/*

144D6CD0   55               PUSH EBP
144D6CD1   8BEC             MOV EBP,ESP
144D6CD3   8B4D 0C          MOV ECX,DWORD PTR SS:[EBP+C]
144D6CD6   81EC 00020000    SUB ESP,200
144D6CDC   8D45 10          LEA EAX,DWORD PTR SS:[EBP+10]
144D6CDF   50               PUSH EAX
144D6CE0   51               PUSH ECX
144D6CE1   8D95 00FEFFFF    LEA EDX,DWORD PTR SS:[EBP-200]
144D6CE7   68 00020000      PUSH 200
144D6CEC   52               PUSH EDX
144D6CED   E8 DEB10000      CALL lua_shar.144E1ED0
144D6CF2   8B45 08          MOV EAX,DWORD PTR SS:[EBP+8]
144D6CF5   8B88 8C100000    MOV ECX,DWORD PTR DS:[EAX+108C]
144D6CFB   8B11             MOV EDX,DWORD PTR DS:[ECX]
144D6CFD   8B52 08          MOV EDX,DWORD PTR DS:[EDX+8]

*/

#define CLuaInterface_ErrorFromLua_Signature "\x55\x8B\xEC\x8B\x4D\x0C\x81\xEC\x00\x02\x00\x00\x8D\x45\x10\x50\x51\x8D\x95\x00\xFE\xFF\xFF\x68\x00\x02\x00\x00\x52\xE8\xDE\xB1\x00\x00\x8B\x45\x08\x8B\x88\x8C\x10\x00\x00\x8B\x11\x8B\x52\x08"
#define CLuaInterface_ErrorFromLua_Mask "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxx????xxxxx"

#endif // GM_LUAERROR_H