#include <stdio.h>
#include <conio.h>
#include <winsock2.h>

#include <bitbuf.h>
#include <convar.h>

#include "cremoteload.h"

const int MAX_COMMAND_PARAMETER_SIZE = 64;
const int MAX_COMMAND_PARAMETERS = 8;

const WORD GAME_CONSOLE_TEXT_COLOUR = 0x07;
const WORD HOST_CONSOLE_TEXT_COLOUR = 0x0A;

HANDLE hostConsole;
HANDLE hostNetListenerThread;
SOCKET hostSocket;
sockaddr_in bridgeAddress;

void HostNetListener()
{
	while ( true )
	{
		char bridgeData[512];
		sockaddr_in bridgeAddress;
		int bridgeAddressSize = sizeof( bridgeAddress );
		int bridgeDataSize = recvfrom( hostSocket, bridgeData, sizeof( bridgeData ), 0, (sockaddr *)&bridgeAddress, &bridgeAddressSize );

		if ( bridgeDataSize == SOCKET_ERROR )
			break;

		bf_read bridgeDataReader( bridgeData, bridgeDataSize );

		int messageType = bridgeDataReader.ReadByte();
		
		//Msg( "Received message %i\n", messageType );

		switch ( messageType )
		{
			case 0:
			{
				char consoleMessage[1024];
				
				if ( bridgeDataReader.ReadString( consoleMessage, sizeof( consoleMessage ) ) )
				{
					if ( !strstr( consoleMessage, "Reference" ) )
					{
						SetConsoleTextAttribute( hostConsole, GAME_CONSOLE_TEXT_COLOUR );
						printf( "%s", consoleMessage );
						SetConsoleTextAttribute( hostConsole, HOST_CONSOLE_TEXT_COLOUR );
					}
				}

				break;
			}
		}
	}

	Msg( "Host net listener error (%i)\n", WSAGetLastError() );
}

bool InitiateHost()
{
	WSADATA wsaData;
		
	if ( WSAStartup( WINSOCK_VERSION, &wsaData ) != 0 )
	{
		printf( "WSAStartup failed (%i)\n", WSAGetLastError() );

		return false;
	}

	hostSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

	sockaddr_in hostAddress;
	hostAddress.sin_family = AF_INET;
	hostAddress.sin_port = htons( 37005 );
	hostAddress.sin_addr.s_addr = inet_addr( "127.0.0.1" );
		
	if ( bind( hostSocket, (const sockaddr *)&hostAddress, sizeof( hostAddress ) ) != 0 )
	{
		printf( "bind failed (%i)\n", WSAGetLastError() );

		return false;
	}

	bridgeAddress.sin_family = AF_INET;
	bridgeAddress.sin_port = htons( 37015 );
	bridgeAddress.sin_addr.s_addr = inet_addr( "127.0.0.1" );

	Msg( "Starting host net listener thread..\n" );

	hostNetListenerThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)&HostNetListener, NULL, 0, NULL );

	return true;
}

void ShutdownHost()
{
	Msg( "Shutting down host..\n" );

	TerminateThread( hostNetListenerThread, 0 );

	closesocket( hostSocket );

	WSACleanup();
}

void InputLoop()
{
	while ( true )
	{
		// User input
		char commandString[MAX_COMMAND_PARAMETER_SIZE*MAX_COMMAND_PARAMETERS];
		fgets( commandString, sizeof( commandString ), stdin );
		int commandLength = (int)strlen( commandString );

		// Check for trailing newline
		if ( commandString[commandLength-1] == '\n' )
			commandString[--commandLength] = '\0';
		
		// Ignore empty commands
		if ( commandLength == 0 )
			continue;

		// Command parameter string (parameters concatenated)
		char commandParametersString[MAX_COMMAND_PARAMETER_SIZE*MAX_COMMAND_PARAMETERS];
		char *commandParametersStart = strstr( commandString, " " );

		if ( commandParametersStart && *(commandParametersStart+1) != '\0' )
			strcpy_s( commandParametersString, MAX_COMMAND_PARAMETER_SIZE*MAX_COMMAND_PARAMETERS, commandParametersStart+1 );
		else
			commandParametersString[0] = '\0'; // no parameters

		// Command parameter array (parameters split into array)
		int commandParametersCount = 0;
		char commandParameters[MAX_COMMAND_PARAMETERS+1][MAX_COMMAND_PARAMETER_SIZE];
		char *commandParameter = strtok( commandString, " " );

		for ( commandParametersCount; commandParametersCount < MAX_COMMAND_PARAMETERS; commandParametersCount++ )
		{
			if ( commandParameter )
			{
				strcpy_s( commandParameters[commandParametersCount], MAX_COMMAND_PARAMETER_SIZE, commandParameter );

				commandParameter = strtok( NULL, " " );
			}
			else
			{
				commandParameters[commandParametersCount][0] = '\0';
			}
		}

		if ( strcmp( commandParameters[0], "cmd" ) == 0 )
		{
			char hostData[512];
			bf_write hostDataWriter( hostData, sizeof( hostData ) );

			hostDataWriter.WriteByte( 0 ); // type (0=game command)
			hostDataWriter.WriteString( commandParametersString ); // game command
			
			if ( sendto( hostSocket, hostData, hostDataWriter.GetNumBytesWritten(), 0, (const sockaddr *)&bridgeAddress, sizeof( bridgeAddress ) ) != SOCKET_ERROR )
				printf( "Sent command [%s]\n", commandParametersString );
			else
				printf( "Failed to send command [%s]\n", commandParametersString );
		}
		else if ( strcmp( commandParameters[0], "exit" ) == 0 )
		{
			break;
		}
		else
		{
			printf( "Unknown command [%s]\n", commandParameters[0] );
		}
	}
}

int main()
{
	// Store console handle
	hostConsole = GetStdHandle( STD_OUTPUT_HANDLE );

	// Set output text colour
	SetConsoleTextAttribute( hostConsole, HOST_CONSOLE_TEXT_COLOUR );

	// Initiate injector
	CRemoteLoad hl2Loader( "hl2.exe" );

	// If the bridge is already loaded, unload it
	hl2Loader.Unload( "sourceconsole_bridge.dll" );

	// Load bridge
	if ( hl2Loader.Load( "sourceconsole_bridge.dll" ) )
	{
		printf( "Injected bridge..\n" );

		// Initate host
		if ( InitiateHost() )
		{
			// Listen for input
			InputLoop();
		}

		// Unload bridge
		hl2Loader.Unload( "sourceconsole_bridge.dll" );

		// Shutdown host
		ShutdownHost();
	}
	else
	{
		printf( "Failed to inject bridge\n" );
	}

	printf( "Press any key to exit.." );
	_getch();

	return 0;
}