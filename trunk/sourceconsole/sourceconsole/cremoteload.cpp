#include "cremoteload.h"

bool EnableDebugPrivilege()
{
	BOOL bReturn = FALSE;
	
	HANDLE hToken;

	if ( OpenProcessToken( GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken ) )
	{
		LUID luid;

		if ( LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &luid ) )
		{
			TOKEN_PRIVILEGES tkp;
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Luid = luid;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			bReturn = AdjustTokenPrivileges( hToken, false, &tkp, sizeof(TOKEN_PRIVILEGES), NULL, NULL );
		}

		CloseHandle( hToken );
	}

	return bReturn == TRUE ? true : false;
}

bool FormatDLLString( const char *pchDLL, char *pchString, bool bUseInjectorPath )
{
	bool bReturn = !bUseInjectorPath;

	if ( bUseInjectorPath )
	{
		_getcwd( pchString, MAX_PATH );

		if ( sprintf_s( pchString, MAX_PATH, "%s\\%s", pchString, pchDLL ) > -1 )
		{
			struct stat info;

			if ( stat( pchString, &info ) == 0 )
			{
				bReturn = true;
			}
		}
	}
	else
	{
		strcpy_s( pchString, MAX_PATH, pchDLL );
	}

	return bReturn;
}

bool CallRemoteFunction( HANDLE hProcess, HMODULE hModule, const char *pchName, DWORD *pdwReturn, void *pvArg )
{
	if ( !hProcess || hProcess == INVALID_HANDLE_VALUE ) return false;
	if ( !hModule || hModule == INVALID_HANDLE_VALUE ) return false;
	if ( !pchName ) return false;
	if ( !pdwReturn ) return false;
	if ( !pvArg ) return false;

	HANDLE hThread = CreateRemoteThread( hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress( hModule, pchName ), pvArg, 0, NULL );

	if ( !hThread || hThread == INVALID_HANDLE_VALUE )
		return false;

	WaitForSingleObject( hThread, INFINITE );
	GetExitCodeThread( hThread, pdwReturn );
	CloseHandle( hThread );

	return true;
}

CRemoteLoad::CRemoteLoad(const char *name)
{
	m_hProcess = NULL;

	if ( !EnableDebugPrivilege() )
		return;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

	if ( !hSnapshot || hSnapshot == INVALID_HANDLE_VALUE )
		return;

	Process32First(hSnapshot, &pe32);

	do
	{
		if ( strcmp( pe32.szExeFile, name ) == NULL )
		{
			m_hProcess = OpenProcess( PROCESS_ALL_ACCESS, false, pe32.th32ProcessID );

			break;
		}
	} while ( Process32Next( hSnapshot, &pe32 ) );
}

CRemoteLoad::~CRemoteLoad()
{
	if ( IsValid() )
		CloseHandle( m_hProcess );
}

DWORD CRemoteLoad::DoLibraryFunction( const char *dll, bool bUseInjectorPath, bool bLoading )
{
	HMODULE hKernel = GetModuleHandle( "kernel32" );	
	DWORD dwReturn = 0;
	char pvMemValue[MAX_PATH];

	if ( !FormatDLLString( dll, pvMemValue, bUseInjectorPath ) )
		return false;

	int pvMemSz = (int)strlen( pvMemValue ) + 1;	
	void *pvMem = VirtualAllocEx( m_hProcess, NULL, pvMemSz, MEM_COMMIT, PAGE_READWRITE );

	if ( pvMem )
	{
		if ( WriteProcessMemory( m_hProcess, pvMem, (void *)pvMemValue, pvMemSz, NULL ) )
		{
			if ( CallRemoteFunction( m_hProcess, hKernel, "GetModuleHandleA", &dwReturn, pvMem ) )
			{
				HMODULE hModule = (HMODULE)dwReturn;

				if ( bLoading )
				{
					if ( hModule == NULL || hModule == INVALID_HANDLE_VALUE )
					{
						CallRemoteFunction( m_hProcess, hKernel, "LoadLibraryA", &dwReturn, pvMem );
					}
				}
				else
				{
					if ( hModule && hModule != INVALID_HANDLE_VALUE )
					{
						CallRemoteFunction( m_hProcess, hKernel, "FreeLibrary", &dwReturn, hModule );
						
						CloseHandle( hModule );
					}
				}
			}
		}

		VirtualFreeEx( m_hProcess, pvMem, pvMemSz, MEM_DECOMMIT );
	}

	return dwReturn;
}

bool CRemoteLoad::Load( const char *dll, bool bUseInjectorPath )
{
	return DoLibraryFunction( dll, bUseInjectorPath, true ) != 0;
}

bool CRemoteLoad::Unload( const char *dll, bool bUseInjectorPath )
{
	return DoLibraryFunction( dll, bUseInjectorPath, false ) == TRUE;
}

bool CRemoteLoad::IsValid()
{
	return m_hProcess && m_hProcess != INVALID_HANDLE_VALUE;
}