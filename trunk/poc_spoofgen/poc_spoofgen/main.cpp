// ------------------------------------------------------------------------------------------
// This generates data that can be used to demonstrate packet spoofing on the Source engine.
// ------------------------------------------------------------------------------------------

#include <stdio.h>
#include <conio.h>
#include <windows.h>

#include "net.h"
#include "bitbuf.h"
#include "bitvec.h"
#include "protocol.h"
#include "checksum_crc.h"
#include "icommandline.h"
#include "strtools.h"

unsigned short CRC16_ProcessSingleBuffer( unsigned char *data, unsigned int size )
{
	int crc32 = CRC32_ProcessSingleBuffer( data, size );

	return (unsigned short)(crc32 ^ ( crc32 >> 16 ));
}

void GenerateMessage( int unOutgoingSequence, int unIncomingSequence, unsigned int unMessageType, bf_write &buffer )
{
	byte flags;
	byte send_data[1024];

	bf_write send( send_data, sizeof( send_data ) );

	send.WriteLong( unOutgoingSequence ); // Outgoing sequence
	send.WriteLong( unIncomingSequence ); // Incoming sequence

	bf_write final = send;

	send.WriteByte( 0 ); // Flags
	send.WriteWord( 0 ); // CRC16
	send.WriteByte( 0 ); // Reliable value
	send.WriteUBitLong( unMessageType, 6 ); // Message
	send.WriteBits( buffer.GetData(), buffer.GetNumBitsWritten() ); // Data

	while ( UPPER_BOUND( send.GetNumBitsWritten() ) < 128 )
		send.WriteUBitLong( net_NOP, 6 );

	if ( ( USED_BYTE_BITS( send.GetNumBitsWritten() ) - 1 ) <= 1 )
		send.WriteUBitLong( net_NOP, 6 );

	int bits_used = USED_BYTE_BITS( send.GetNumBitsWritten() );

	if ( bits_used > 0 )
	{
		int bits_free = 8 - bits_used;

		flags |= ENCODE_PAD_BITS( bits_free );

		if ( bits_free > 0 )
			send.WriteUBitLong( GetBitForBitnum( bits_free ) - 1, bits_free );
	}

	final.WriteByte( flags );
	final.WriteWord( CRC16_ProcessSingleBuffer( send.GetData() + 11, send.GetNumBytesWritten() - 11 ) );

	char hex[1024];
	V_binarytohex( send.GetData(), send.GetNumBytesWritten(), hex, sizeof( hex ) );

	FILE *hFile = fopen( "out.txt", "wb" );

	if ( hFile )
	{
		fwrite( hex, sizeof( char ), strlen( hex ), hFile );
		fclose( hFile );
	}
}

int main( int argc, char **argv )
{
	CommandLine()->CreateCmdLine( argc, argv );

	unsigned int unOutgoingSequence = CommandLine()->ParmValue( "-out_seq", -1 );

	if ( unOutgoingSequence == -1 )
	{
		printf( "Missing parameter -out_seq\n" );

		return 0;
	}

	unsigned int unIncomingSequence = CommandLine()->ParmValue( "-inc_seq", -1 );

	if ( unIncomingSequence == -1 )
	{
		printf( "Missing parameter -inc_seq\n" );

		return 0;
	}

	const char *pszMessageType = CommandLine()->ParmValue( "-message" );

	if ( !pszMessageType )
	{
		printf( "Missing parameter -message\n" );

		return 0;
	}

	unsigned char message_data[512];
	bf_write message( message_data, sizeof( message_data ) );

	unsigned int unMessageType = net_NOP;

	if ( strcmp( strlwr( (char *)pszMessageType ), "net_stringcmd" ) == 0 )
	{
		unMessageType = net_StringCmd;

		const char *pchCommand = CommandLine()->ParmValue( "-command" );

		if ( !pchCommand )
		{
			printf( "Missing message parameter -command\n" );

			return 0;
		}

		message.WriteString( pchCommand );
	}
	else
	{
		printf( "Invalid parameter -message\n" );

		return 0;
	}

	if ( unMessageType != net_NOP )
		GenerateMessage( unOutgoingSequence, unIncomingSequence, unMessageType, message );

	return 0;
}