// Project headers
#include "client.h"
#include "config.h"
#include "net_helpers.h"

// Components
#include "component_steamapi.h"
#include "component_sourcenet.h"

// SDK headers
#include "bitbuf.h"
#include "bitvec.h"
#include "dbg.h"

// OSW headers
#undef NETADR_H
#include "steamapi.h"
#include "steamclient.h"

bool CL_ProcessConnectionlessPacket( bf_read &buf )
{
	char c = buf.ReadByte();

	switch ( c )
	{
		case '9': // S2C_CONNREJECT
		{
			if ( buf.ReadLong() != net_client_challenge )
			{
				Msg( "S2C_CONNREJECT: client challenge mismatch\n" );

				return false;
			}
			
			char szReason[1260];
				
			if ( buf.ReadString( szReason, sizeof( szReason ) ) )
			{
				Msg( "Dropped by server (reason: %s)\n", szReason );

				return false;
			}

			return false; // disconnect
		}
		case 'A': // S2C_CHALLENGE
		{
			if ( buf.ReadLong() != MAGIC_VERSION )
			{
				Msg( "S2C_CHALLENGE: version mismatch\n" );

				return false;
			}

			net_server_challenge = buf.ReadLong();

			if ( buf.ReadLong() != net_client_challenge )
			{
				Msg( "S2C_CHALLENGE: client challenge mismatch\n" );

				return false;
			}

			if ( buf.ReadLong() != PROTOCOL_STEAM )
			{
				Msg( "S2C_CHALLENGE: unsupported protocol\n" );

				return false;
			}

			if ( buf.ReadShort() != 0 )
			{
				Msg( "S2C_CHALLENGE: invalid Steam key size\n" );

				return false;
			}

			uint64 gsID = 0; // Game server Steam ID

			bool bSecure = false; // VAC switch

			if ( buf.GetNumBytesLeft() > 8 )
			{
				if ( !buf.ReadBytes( &gsID, sizeof( gsID ) ) )
				{
					Msg( "S2C_CHALLENGE: invalid GS Steam ID\n" );

					return false;
				}

				bSecure = buf.ReadByte() ? true : false;
			}

			net_datagram.WriteLong( -1 );
			net_datagram.WriteByte( 'k' );
			net_datagram.WriteLong( PROTOCOL_VERSION );
			net_datagram.WriteLong( PROTOCOL_STEAM );
			net_datagram.WriteLong( net_server_challenge );
			net_datagram.WriteLong( net_client_challenge );
			net_datagram.WriteString( "" );
			net_datagram.WriteString( "" );
			net_datagram.WriteString( net_game_version );

			unsigned char uchSteamKey[STEAM_KEYSIZE];
			int nSteamKeySize = g_pSteamUser->InitiateGameConnection( uchSteamKey, sizeof( uchSteamKey ), CSteamID( gsID ), CGameID( net_game_id ), net_server_adr.GetIP(), net_server_adr.GetPort(), bSecure );

			net_datagram.WriteShort( nSteamKeySize );

			if ( nSteamKeySize )
				net_datagram.WriteBytes( uchSteamKey, nSteamKeySize );

			g_pSteamUser->TerminateGameConnection( net_server_adr.GetIP(), net_server_adr.GetPort() );

			return NET_SendDatagram();
		}
		case 'B': // S2C_CONNECTION
		{
			Msg( "Connected to server.\n" );

			byte flags = 0;

			net_datagram.WriteLong( 1 ); // outgoing sequence
			net_datagram.WriteLong( 1 ); // incoming sequence
			
			bf_write final = net_datagram;

			net_datagram.WriteByte( 0 ); // flags
			net_datagram.WriteWord( 0 ); // crc16
			net_datagram.WriteByte( 0 ); // reliable value

			net_datagram.WriteUBitLong( clc_CmdKeyValues, 6 ); // net message
			net_datagram.WriteLong( 0x7FFFFFFF ); // alloc size

			while ( UPPER_BOUND( net_datagram.GetNumBitsWritten() ) < 128 )
				net_datagram.WriteUBitLong( net_NOP, 6 );

			if ( ( USED_BYTE_BITS( net_datagram.GetNumBitsWritten() ) - 1 ) <= 1 )
				net_datagram.WriteUBitLong( net_NOP, 6 );

			int bits_used = USED_BYTE_BITS( net_datagram.GetNumBitsWritten() );

			if ( bits_used > 0 )
			{
				int bits_free = 8 - bits_used;

				flags |= ENCODE_PAD_BITS( bits_free );

				if ( bits_free > 0 )
					net_datagram.WriteUBitLong( GetBitForBitnum( bits_free ) - 1, bits_free );
			}

			final.WriteByte( flags );
			final.WriteWord( CRC16_ProcessSingleBuffer( net_datagram.GetData() + final.GetNumBytesWritten() + sizeof( short ), net_datagram.GetNumBytesWritten() - final.GetNumBytesWritten() - sizeof( short ) ) );

			if ( NET_SendDatagram() )
				Msg( "Sent clc_CmdKeyValues packet.\n" );

			return false; // disconnect
		}
		default:
		{
			Msg( "Received unknown connectionless packet '%c'\n", c );

			return false;
		}
	}
}