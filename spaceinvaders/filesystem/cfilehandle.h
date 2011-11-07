#ifndef CFILEHANDLE_H
#define CFILEHANDLE_H

#include "ifilehandle.h"

#include <windows.h>

class CFileHandle : public IFileHandle
{
public:
	// Constructor
	CFileHandle( const char *name );
	// IFileHandle
	void Close();
	unsigned int GetSize();
	const char *GetName();
	unsigned int Read( char *data, unsigned int bytes );
	unsigned int ReadAll( char *data, unsigned int maxbytes );
	unsigned int Write( char *data, unsigned int bytes, bool append );
private:
	char m_FileName[MAX_PATH];
};

#endif // CFILEHANDLE_H