#include "cfilesystem.h"
#include "cfilehandle.h"

#include <windows.h>

IFileHandle *CFileSystem::Open( const char *name )
{
	return new CFileHandle( name );
}