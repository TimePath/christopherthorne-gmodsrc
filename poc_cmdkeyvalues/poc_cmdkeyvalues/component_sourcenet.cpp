// Project headers
#include "client.h"
#include "config.h"

// Components
#include "component_sourcenet.h"
#include "component_steamapi.h"

// SDK headers
#include "icommandline.h"
#include "dbg.h"

// OSW headers
#include "ISteamUser009.h"

// Client/server info
char net_game_version[128];
unsigned int net_game_id;

netadr_t net_client_adr;
netadr_t net_server_adr;

int net_client_challenge = 0x7FFFFFFF;
int net_server_challenge;

// Timeout
int net_timeout;

// Socket
SOCKET net_socket;

// Net buffer
static char net_buffer[1260];
bf_write net_datagram( net_buffer, sizeof( net_buffer ) );

bool NET_SendDatagram()
{
	sockaddr to;
	net_server_adr.ToSockadr( &to );

	if ( sendto( net_socket, net_buffer, net_datagram.GetNumBytesWritten(), 0, &to, sizeof( to ) ) <= 0 )
		return false;

	net_datagram.SeekToBit( 0 );

	return true;
}

bool NET_ReceiveDatagram( const char *buf, int buflen )
{
	bf_read reply( buf, buflen );

	int pcktype = reply.ReadLong();

	if ( pcktype == -1 )
	{
		return CL_ProcessConnectionlessPacket( reply );
	}
	else
	{
		Msg( "Received non-OOB packet\n" );

		return false;
	}

	return true;
}

REGISTER_COMPONENT_INIT( SourceNet )
{
	// WinSock

	WSAData wsadata;

	if ( WSAStartup( WINSOCK_VERSION, &wsadata ) != 0 )
		return false;

	// Version

	strcpy_s( net_game_version, sizeof( net_game_version ), CommandLine()->ParmValue( "-game_version", DEFAULT_VERSION ) );

	// App ID

	net_game_id = CommandLine()->ParmValue( "-game_id", 0 );

	if ( !net_game_id )
	{
		Msg( "cmdkeyvalues: missing -game_id\n" );

		return false;
	}

	// Server address

	const char *pszServerIP = CommandLine()->ParmValue( "-sv_ip" );

	if ( !pszServerIP )
	{
		Msg( "cmdkeyvalues: missing -sv_ip\n" );

		return false;
	}

	net_server_adr.SetFromString( pszServerIP, true );
	net_server_adr.SetPort( CommandLine()->ParmValue( "-sv_port", SERVER_PORT ) );

	// Client address

	const char *pszClientIP = CommandLine()->ParmValue( "-cl_ip" );

	if ( !pszClientIP )
	{
		char hostname[256];

		if ( gethostname( hostname, sizeof( hostname ) ) != 0 )
			return false;

		hostent *host = gethostbyname( hostname );

		if ( !host )
			return false;

		in_addr *addr = (in_addr *)host->h_addr_list[0];

		if ( !addr )
			return false;

		pszClientIP = inet_ntoa( *addr );
	}	

	net_client_adr.SetFromString( pszClientIP );
	net_client_adr.SetPort( CommandLine()->ParmValue( "-cl_port", CLIENT_PORT ) );
	
	Msg( "Client address: %s\n", net_client_adr.ToString() );
	Msg( "Server address: %s\n", net_server_adr.ToString() );

	// Socket

	net_socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

	if ( net_socket == INVALID_SOCKET )
		return false;

	sockaddr name;	
	net_client_adr.ToSockadr( &name );

	if ( bind( net_socket, &name, sizeof( sockaddr ) ) != 0 )
		return false;

	// Timeout

	net_timeout = CommandLine()->ParmValue( "-timeout", TIMEOUT_SECONDS );

	return true;
}

REGISTER_COMPONENT_SHUTDOWN( SourceNet )
{
	WSACleanup();
}