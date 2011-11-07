#include "cfilehandle.h"

#include <sys/stat.h>
#include <stdio.h>

CFileHandle::CFileHandle( const char *name )
{
	strcpy_s( m_FileName, sizeof( m_FileName ), name );
}

void CFileHandle::Close()
{
	delete this;
}

unsigned int CFileHandle::GetSize()
{
	struct stat info;

	if ( stat( GetName(), &info ) != 0 )
		return 0;

	return info.st_size;
}

const char *CFileHandle::GetName()
{
	return m_FileName;
}

unsigned int CFileHandle::Read( char *data, unsigned int bytes )
{
	FILE *fh;
	
	if ( fopen_s( &fh, GetName(), "rb" ) != 0 )
		return 0;

	unsigned int r = (unsigned int)fread( data, sizeof( char ), bytes, fh );

	fclose( fh );

	return r;
}

unsigned int CFileHandle::ReadAll( char *data, unsigned int maxbytes )
{
	unsigned int size = GetSize();

	return Read( data, size > maxbytes ? maxbytes : size );
}

unsigned int CFileHandle::Write( char *data, unsigned int bytes, bool append )
{
	FILE *fh;
	
	if ( fopen_s( &fh, GetName(), append ? "ab" : "wb" ) != 0 )
		return 0;

	unsigned int r = (unsigned int)fwrite( data, sizeof( char ), bytes, fh );
	
	fclose( fh );

	return r;
}