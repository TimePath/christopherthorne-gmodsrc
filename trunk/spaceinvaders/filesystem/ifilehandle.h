#ifndef IFILEHANDLE_H
#define IFILEHANDLE_H

class IFileHandle
{
public:
	// Frees file handle
	virtual void Close() = 0;
	// Gets file size (in bytes)
	virtual unsigned int GetSize() = 0;
	// Gets file name
	virtual const char *GetName() = 0;
	// Reads specified amount of file data
	virtual unsigned int Read( char *data, unsigned int bytes ) = 0;
	// Reads all file data, unless the buffer is too small
	virtual unsigned int ReadAll( char *data, unsigned int maxbytes ) = 0;
	// Writes file data
	virtual unsigned int Write( char *data, unsigned int bytes, bool append = false ) = 0;
};

#endif // IFILEHANDLE_H