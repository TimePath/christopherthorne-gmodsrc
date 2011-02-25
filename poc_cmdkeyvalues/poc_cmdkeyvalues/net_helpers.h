#ifndef _NET_H
#define _NET_H

// Increases and removes last 3 bits
#define UPPER_BOUND( x ) ( ( x + 7 ) & ~7 )
// Calculates amount of bits written in current byte
#define USED_BYTE_BITS( x ) ( x % 8 )
// Aligns n to dword boundary
#define ALIGN_DWORD( n ) 4 * ( ( n + 3 ) >> 2 )
// Padding
#define ENCODE_PAD_BITS( x ) ( ( x << 5 ) & 0xff )
#define DECODE_PAD_BITS( x ) ( ( x >> 5 ) & 0xff )
// CRC16
unsigned short CRC16_ProcessSingleBuffer( unsigned char *data, unsigned int size );

#endif // _NET_H