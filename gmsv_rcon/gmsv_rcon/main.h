// Headers

#include "GMLuaModule.h"
#include "sv_rcon.h"
#include "cbytescanner.h"
#include "csimpledetour.h"

#include <netadr.h>
#include <bitbuf.h>

// Metadata

#define CRCONSERVER_META_NAME "CRConServer"
#define CRCONSERVER_META_ID 5000

// Engine library

#define ENGINE_LIB "engine.dll"

// Hook helpers

#define CLASSFUNC __stdcall
#define STORETHISPTR() void *_this;__asm MOV _this, ECX

// Signatures

#define CServerRemoteAccess_LogCommand_Signature \
	"\x55" \
	"\x8B\xEC" \
	"\xA1\xB4\x25\x7D\x61" \
	"\x83\x78\x30\x00" \
	"\x74\x51" \
	"\x8B\x45\x08" \
	"\x3B\x41\x44"

#define CServerRemoteAccess_LogCommand_Mask "xxxx????xxxxx?xxxxxx"

#define CServerRemoteAccess_WriteDataRequest_Signature \
	"\x55" \
	"\x8B\xEC" \
	"\x8B\x45\x14" \
	"\x81\xEC\x68\x03\x00\x00" \
	"\x57" \
	"\x8B\xF9" \
	"\x01\x47\x60"

#define CServerRemoteAccess_WriteDataRequest_Mask "xxxxxxxx????xxxxxx"

#define CServerRemoteAccess_CheckPassword_Signature \
	"\x55" \
	"\x8B\xEC" \
	"\x8B\x45\x14" \
	"\x53" \
	"\x56" \
	"\x8B\x75\x08" \
	"\x8B\xD9" \
	"\x50" \
	"\x8B\xCE"

#define CServerRemoteAccess_CheckPassword_Mask "xxxxxxxxxxxxxxxx"

#define CRConServer_IsPassword_Signature \
	"\x55" \
	"\x8B\xEC" \
	"\x56" \
	"\x8D\x71\x40" \
	"\x8B\xCE"

#define CRConServer_IsPassword_Mask "xxxxxxxxx"

// Hooks

void ( CLASSFUNC *CServerRemoteAccess_LogCommand_T )( unsigned int listener, const char *msg );
void CLASSFUNC CServerRemoteAccess_LogCommand_H( unsigned int listener, const char *msg );

void ( CLASSFUNC *CServerRemoteAccess_WriteDataRequest_T )( CRConServer *conn, unsigned int listener, const void *pData, int len );
void CLASSFUNC CServerRemoteAccess_WriteDataRequest_H( CRConServer *conn, unsigned int listener, const void *pData, int len );

void ( CLASSFUNC *CServerRemoteAccess_CheckPassword_T)( CRConServer *conn, unsigned int listener, int id, const char *password );
void CLASSFUNC CServerRemoteAccess_CheckPassword_H( CRConServer *conn, unsigned int listener, int id, const char *password );

bool ( CLASSFUNC *CRConServer_IsPassword_T)( const char *password );
bool CLASSFUNC CRConServer_IsPassword_H( const char *password );