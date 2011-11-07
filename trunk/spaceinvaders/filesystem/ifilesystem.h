#ifndef IFILESYSTEM_H
#define IFILESYSTEM_H

#include "ifilehandle.h"

class IFileSystem
{
public:
	// Allocates a file handle
	virtual IFileHandle *Open( const char *name ) = 0;
};

#endif // IFILESYSTEM_H