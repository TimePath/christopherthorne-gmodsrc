#include "debug.h"

#include <windows.h>

void Error( const char *msg )
{
	MessageBox( NULL, msg, "Error", MB_ICONEXCLAMATION );
}