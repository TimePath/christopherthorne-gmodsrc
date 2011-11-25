#include <winsock2.h>

#include <bitbuf.h>
#include <eiface.h>

HANDLE bridgeNetListenerThread;
SOCKET bridgeSocket;
sockaddr_in hostAddress;

SpewOutputFunc_t previousSpewHandler;

IVEngineServer *engineServer = NULL;

void BridgeNetListener()
{
	while ( true )
	{
		char hostData[512];
		sockaddr_in hostAddress;
		int hostAddressSize = sizeof( hostAddress );
		int hostDataSize = recvfrom( bridgeSocket, hostData, sizeof( hostData ), 0, (sockaddr *)&hostAddress, &hostAddressSize );
		
		if ( hostDataSize == SOCKET_ERROR )
			break;

		bf_read hostDataReader( hostData, hostDataSize );

		int messageType = hostDataReader.ReadByte();
		
		//Msg( "[sourceconsole] Received message %i\n", messageType );

		switch ( messageType )
		{
			case 0:
			{
				char gameCommand[256];
				
				if ( hostDataReader.ReadString( gameCommand, sizeof( gameCommand ) ) )
				{
					int gameCommandLength = (int)strlen( gameCommand );

					Msg( "[sourceconsole] Received command: %s\n", gameCommand );

					if ( gameCommandLength < sizeof( gameCommand ) - 1 )
					{
						gameCommand[gameCommandLength] = '\n';
						gameCommand[gameCommandLength+1] = '\0';

						engineServer->ServerCommand( gameCommand );
					}
					else
					{
						Msg( "[sourceconsole] Game command too long\n" );
					}					
				}
				else
				{
					Msg( "[sourceconsole] Failed to read string\n" );
				}

				break;
			}
		}
	}

	Msg( "[sourceconsole] Bridge net listener error (%i)\n", WSAGetLastError() );
}

SpewRetval_t BridgeSpewHandler( SpewType_t spewType, const tchar *pMsg )
{
	if ( strstr( pMsg, "[sourceconsole]" ) == NULL )
	{
		char bridgeData[1024];
		bf_write bridgeDataWriter( bridgeData, sizeof( bridgeData ) );

		bridgeDataWriter.WriteByte( 0 );
		bridgeDataWriter.WriteString( pMsg );

		sendto( bridgeSocket, bridgeData, bridgeDataWriter.GetNumBytesWritten(), 0, (const sockaddr *)&hostAddress, sizeof( hostAddress ) );
	}

	return previousSpewHandler( spewType, pMsg );
}

bool InititateBridge()
{
	Msg( "[sourceconsole] Initiating bridge..\n" );

	CreateInterfaceFn engineFactory = Sys_GetFactory( "engine.dll" );

	engineServer = (IVEngineServer *)engineFactory( INTERFACEVERSION_VENGINESERVER, NULL );

	WSADATA wsaData;
	
	if ( WSAStartup( WINSOCK_VERSION, &wsaData ) != 0 )
	{
		Msg( "[sourceconsole] Failed to create bridge listener socket (startup error)\n" );
		
		return false;		
	}

	bridgeSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

	sockaddr_in bridgeAddress;
	bridgeAddress.sin_family = AF_INET;
	bridgeAddress.sin_port = htons( 37015 );
	bridgeAddress.sin_addr.s_addr = inet_addr( "127.0.0.1" );
		
	if ( bind( bridgeSocket, (const sockaddr *)&bridgeAddress, sizeof( bridgeAddress ) ) != 0 )
	{
		Msg( "[sourceconsole] Failed to create bridge listener socket (bind error %i)\n", WSAGetLastError() );
		
		return false;
	}

	hostAddress.sin_family = AF_INET;
	hostAddress.sin_port = htons( 37005 );
	hostAddress.sin_addr.s_addr = inet_addr( "127.0.0.1" );

	Msg( "[sourceconsole] Starting bridge net listener thread..\n" );

	bridgeNetListenerThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)&BridgeNetListener, NULL, 0, NULL );

	previousSpewHandler = GetSpewOutputFunc();

	SpewOutputFunc( BridgeSpewHandler );

	return true;
}

void ShutdownBridge()
{
	Msg( "[sourceconsole] Shutting down bridge..\n" );

	TerminateThread( bridgeNetListenerThread, 0 );

	SpewOutputFunc( previousSpewHandler );

	closesocket( bridgeSocket );

	WSACleanup();
}

BOOL WINAPI DllMain( HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved )
{
	switch ( fdwReason )
	{
	case DLL_PROCESS_ATTACH:
		return InititateBridge();
	case DLL_PROCESS_DETACH:
		ShutdownBridge();
		break;
	default:
		break;
	}

	return TRUE;
}