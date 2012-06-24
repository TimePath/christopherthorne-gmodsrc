#include "main.h"

GMOD_MODULE( Open, Close );

ILuaInterface *g_Lua = NULL;

netadr_s *passwordCheckAddress = NULL;

SETUP_SIMPLE_DETOUR( CServerRemoteAccess_LogCommand_Detour, CServerRemoteAccess_LogCommand_T, CServerRemoteAccess_LogCommand_H );
SETUP_SIMPLE_DETOUR( CServerRemoteAccess_WriteDataRequest_Detour, CServerRemoteAccess_WriteDataRequest_T, CServerRemoteAccess_WriteDataRequest_H );
SETUP_SIMPLE_DETOUR( CServerRemoteAccess_CheckPassword_Detour, CServerRemoteAccess_CheckPassword_T, CServerRemoteAccess_CheckPassword_H );
SETUP_SIMPLE_DETOUR( CRConServer_IsPassword_Detour, CRConServer_IsPassword_T, CRConServer_IsPassword_H );

netadr_s *ListenerIDToAddress( CServerRemoteAccess *c, unsigned int id )
{
	unsigned int pos = (id*8)-id;

	netadr_s *addr;

	__asm
	{
		PUSH ECX
		PUSH EAX
		PUSH EDX
		MOV ECX, c
		MOV EAX, [ECX+0x2C]
		MOV ECX, pos
		LEA EDX, [EAX+ECX*0x04]
		LEA EAX, [EDX+0x08]
		MOV addr, EAX
		POP EDX
		POP EAX
		POP ECX
	}

	return addr;
}

void CLASSFUNC CServerRemoteAccess_LogCommand_H( unsigned int listener, const char *msg )
{
	STORETHISPTR();

	// Sender address

	netadr_s *addr = ListenerIDToAddress( (CServerRemoteAccess *)_this, listener );

	if ( addr )
	{
		// Call hook

		ILuaObject *hookTable = g_Lua->GetGlobal( "hook" );
		ILuaObject *hookCall = hookTable->GetMember( "Call" );
		
		hookCall->Push();
		g_Lua->Push( "RCON_LogCommand" );
		g_Lua->PushNil();
		g_Lua->Push( msg );
		g_Lua->Push( addr->ToString( true ) );
		g_Lua->PushDouble( addr->GetPort() );
		g_Lua->Call( 5, 1 );
		
		hookCall->UnReference();
		hookTable->UnReference();

		// Check return value

		ILuaObject *returnValue = g_Lua->GetReturn( 0 );
		
		bool returnIsNil = returnValue->isNil();

		returnValue->UnReference();
		
		// Don't call original function if a value is returned
		if ( !returnIsNil )
			return;
	}

	// Call original function
	__asm
	{
		PUSH msg
		PUSH listener
		MOV ECX, _this
		CALL CServerRemoteAccess_LogCommand_T
	}
}

void CLASSFUNC CServerRemoteAccess_WriteDataRequest_H( CRConServer *conn, unsigned int listener, const void *pData, int len )
{
	STORETHISPTR();

	// Sender address

	netadr_s *addr = ListenerIDToAddress( (CServerRemoteAccess *)_this, listener );

	if ( addr )
	{
		// Sender data

		bf_read buffer( pData, len );
		long id = buffer.ReadLong();
		long request = buffer.ReadLong();
		char str[1024];
		buffer.ReadString( str, 1024 );

		if ( !buffer.IsOverflowed() )
		{
			// Call hook

			ILuaObject *hookTable = g_Lua->GetGlobal( "hook" );
			ILuaObject *hookCall = hookTable->GetMember( "Call" );

			hookCall->Push();
			g_Lua->Push( "RCON_WriteDataRequest" );
			g_Lua->PushNil();
			g_Lua->PushDouble( id );
			g_Lua->PushDouble( request );
			g_Lua->Push( str );
			g_Lua->Push( addr->ToString( true ) );
			g_Lua->PushDouble( addr->GetPort() );
			g_Lua->Call( 7, 1 );
			
			hookCall->UnReference();
			hookTable->UnReference();

			// Check return value

			ILuaObject *returnValue = g_Lua->GetReturn( 0 );
			
			bool returnIsNil = returnValue->isNil();
		
			returnValue->UnReference();

			// Don't call original function if a value is returned
			if ( !returnIsNil )
				return;
		}
	}

	// Call original function
	__asm
	{
		PUSH len
		PUSH pData
		PUSH listener
		PUSH conn
		MOV ECX, _this
		CALL CServerRemoteAccess_WriteDataRequest_T
	}
}

void CLASSFUNC CServerRemoteAccess_CheckPassword_H( CRConServer *conn, unsigned int listener, int id, const char *password )
{
	STORETHISPTR();

	// Store address for CRConServer::IsPassword
	// Password checking can't be done in this function because it doesn't have a return value
	passwordCheckAddress = ListenerIDToAddress( (CServerRemoteAccess *)_this, listener );

	// Call original function
	__asm
	{
		PUSH password
		PUSH id
		PUSH listener
		PUSH conn
		MOV ECX, _this
		CALL CServerRemoteAccess_CheckPassword_T
	}
}

bool CLASSFUNC CRConServer_IsPassword_H( const char *password )
{
	STORETHISPTR();

	if ( passwordCheckAddress )
	{
		// Call hook

		ILuaObject *hookTable = g_Lua->GetGlobal( "hook" );
		ILuaObject *hookCall = hookTable->GetMember( "Call" );
		
		hookCall->Push();
		g_Lua->Push( "RCON_CheckPassword" );
		g_Lua->PushNil();
		g_Lua->Push( password );
		g_Lua->Push( passwordCheckAddress->ToString( true ) );
		g_Lua->PushDouble( passwordCheckAddress->GetPort() );
		g_Lua->Call( 5, 1 );
		
		hookCall->UnReference();
		hookTable->UnReference();

		// Check return value

		ILuaObject *returnValue = g_Lua->GetReturn( 0 );
		
		bool returnIsNil = returnValue->isNil();	
		bool returnResponse = returnValue->GetBool();
		
		returnValue->UnReference();

		// Reset stored address
		passwordCheckAddress = NULL;

		// Return custom password response if specified
		if ( !returnIsNil )
			return returnResponse;
	}

	// Call original function
	__asm
	{
		PUSH password
		MOV ECX, _this
		CALL CRConServer_IsPassword_T
	}
}

int Open( lua_State *L )
{
	g_Lua = Lua();

	CByteScanner engineScanner( ENGINE_LIB );

	// CServerRemoteAccess::LogCommand

	if ( engineScanner.FindCodePattern( CServerRemoteAccess_LogCommand_Signature, CServerRemoteAccess_LogCommand_Mask, (LPVOID *)&CServerRemoteAccess_LogCommand_T ) )
		CServerRemoteAccess_LogCommand_Detour.Attach();
	else
		g_Lua->ErrorNoHalt( "[gmsv_rcon] CServerRemoteAccess::LogCommand signature scan failed. Please report this error.\n" );

	// CServerRemoteAccess::WriteDataRequest

	if ( engineScanner.FindCodePattern( CServerRemoteAccess_WriteDataRequest_Signature, CServerRemoteAccess_WriteDataRequest_Mask, (LPVOID *)&CServerRemoteAccess_WriteDataRequest_T ) )
		CServerRemoteAccess_WriteDataRequest_Detour.Attach();
	else
		g_Lua->ErrorNoHalt( "[gmsv_rcon] CServerRemoteAccess::WriteDataRequest signature scan failed. Please report this error.\n" );

	// CServerRemoteAccess::CheckPassword

	if ( engineScanner.FindCodePattern( CServerRemoteAccess_CheckPassword_Signature, CServerRemoteAccess_CheckPassword_Mask, (LPVOID *)&CServerRemoteAccess_CheckPassword_T ) )
		CServerRemoteAccess_CheckPassword_Detour.Attach();
	else
		g_Lua->ErrorNoHalt( "[gmsv_rcon] CServerRemoteAccess::CheckPassword signature scan failed. Please report this error.\n" );

	// CRConServer::IsPassword

	if ( engineScanner.FindCodePattern( CRConServer_IsPassword_Signature, CRConServer_IsPassword_Mask, (LPVOID *)&CRConServer_IsPassword_T ) )
		CRConServer_IsPassword_Detour.Attach();
	else
		g_Lua->ErrorNoHalt( "[gmsv_rcon] CRConServer::IsPassword signature scan failed. Please report this error.\n" );

	return 0;
}

int Close( lua_State *L )
{
	CServerRemoteAccess_LogCommand_Detour.Detach();
	CServerRemoteAccess_WriteDataRequest_Detour.Detach();
	CServerRemoteAccess_CheckPassword_Detour.Detach();
	CRConServer_IsPassword_Detour.Detach();

	return 0;
}