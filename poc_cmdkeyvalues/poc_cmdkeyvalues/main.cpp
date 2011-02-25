// Components
#include "component_steamapi.h"
#include "component_sourcenet.h"

// C/C++ headers
#include <stdio.h>

// SDK headers
#include "icommandline.h"
#include "dbg.h"
#include "netadr.h"
#include "bitbuf.h"

bool InitComponents()
{
	INIT_COMPONENT( SteamAPI );
	INIT_COMPONENT( SourceNet );

	return true;
}

void ShutdownComponents()
{
	SHUTDOWN_COMPONENT( SourceNet );
	SHUTDOWN_COMPONENT( SteamAPI );
}

void ConnectToServer()
{
	Msg( "Connecting to %s..\n", net_server_adr.ToString() );

	net_datagram.WriteLong( -1 );
	net_datagram.WriteByte( 'q' ); // A2A_GETCHALLENGE
	net_datagram.WriteLong( net_client_challenge );
	net_datagram.WriteString( "0000000000" );

	if ( !NET_SendDatagram() )
		return;

	fd_set readfds;

	FD_ZERO( &readfds );
	FD_SET( net_socket, &readfds );

	timeval timeout;
	
	timeout.tv_sec = net_timeout;
	timeout.tv_usec = 0;

	while ( true )
	{
		char buf[2048]; // buffer
		int buflen; // buffer size
		sockaddr_in from; // src
		int fromlen = sizeof( sockaddr_in ); // src size

		if ( select( -1, &readfds, NULL, NULL, &timeout ) <= 0 )
		{
			Msg( "Connection timed out.\n" );

			break;
		}

		if ( ( buflen = recvfrom( net_socket, buf, sizeof( buf ), 0, (sockaddr *)&from, &fromlen ) ) <= 0 )
		{
			Msg( "Server is not responding.\n" );

			break;
		}

		netadr_t src;

		if ( !src.SetFromSockadr( (const sockaddr *)&from ) || !( src == net_server_adr ) )
			continue;

		if ( !NET_ReceiveDatagram( buf, buflen ) )
			break;
	}
}

int main( int argc, char **argv )
{
	CommandLine()->CreateCmdLine( argc, argv );

	if ( InitComponents() )
		ConnectToServer();

	ShutdownComponents();

	return 0;
}