// Projects headers
#include "net_helpers.h"

// SDK headers
#include "checksum_crc.h"

unsigned short CRC16_ProcessSingleBuffer( unsigned char *data, unsigned int size )
{
	int crc32 = CRC32_ProcessSingleBuffer( data, size );

	return (unsigned short)(crc32 ^ ( crc32 >> 16 ));
}