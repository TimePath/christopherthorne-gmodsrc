#ifndef _NET_H
#define _NET_H

// Increases and removes last 3 bits
#define UPPER_BOUND( x ) ( ( x + 7 ) & ~7 )
// Calculates amount of bits written in current byte
#define USED_BYTE_BITS( x ) ( x % 8 )
// Aligns n to dword boundary
#define ALIGN_DWORD(n) 4 * ( ( n + 3 ) >> 2 )

#endif // _NET_CHAN