#ifndef CREMOTELOAD_H
#define CREMOTELOAD_H

#include <windows.h>
#include <tlhelp32.h>
#include <direct.h>
#include <sys/stat.h>
#include <stdio.h>

class CRemoteLoad
{
public:
	CRemoteLoad( const char *name );
	~CRemoteLoad();

	bool Load( const char *dll, bool bUseInjectorPath = true );
	bool Unload( const char *dll, bool bUseInjectorPath = true );
	bool IsValid();

protected:
	DWORD DoLibraryFunction( const char *dll, bool bUseInjectorPath, bool bLoading );

private:
	HANDLE m_hProcess;
};

#endif //CREMOTELOAD_H