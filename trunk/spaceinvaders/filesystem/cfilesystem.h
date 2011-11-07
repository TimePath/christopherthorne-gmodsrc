#ifndef CFILESYSTEM_H
#define CFILESYSTEM_H

#include "ifilesystem.h"

class CFileSystem : public IFileSystem
{
public:
	// IFileSystem
	IFileHandle *Open( const char *name );
};

#endif // CFILESYSTEM_H