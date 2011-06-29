#include "cbytescanner.h"

CByteScanner::CByteScanner()
{
	m_dwBaseOfCode = NULL;
	m_dwSizeOfCode = 0;
}

CByteScanner::CByteScanner( LPCVOID lpAddress )
{
	m_dwBaseOfCode = NULL;
	m_dwSizeOfCode = 0;

	FindCodeSegment( lpAddress );
}

CByteScanner::CByteScanner( LPCSTR lpModuleName )
{
	m_dwBaseOfCode = NULL;
	m_dwSizeOfCode = 0;

	FindCodeSegment( GetModuleHandle( lpModuleName ) );
}

BOOL CByteScanner::FindCodeSegment( LPCVOID lpAddress )
{
	if ( !lpAddress )
		return FALSE;

	MEMORY_BASIC_INFORMATION mbi;

	if ( VirtualQuery( lpAddress, &mbi, sizeof( mbi ) ) == 0 )
		return FALSE;

	DWORD dwAllocationBase = (DWORD)mbi.AllocationBase;

	if ( !dwAllocationBase )
		return FALSE;

	IMAGE_DOS_HEADER *idh = (IMAGE_DOS_HEADER *)(dwAllocationBase);
	IMAGE_NT_HEADERS *inh = (IMAGE_NT_HEADERS *)(dwAllocationBase+idh->e_lfanew);

	if ( !inh->OptionalHeader.BaseOfCode )
		return FALSE;

	m_dwBaseOfCode = dwAllocationBase + inh->OptionalHeader.BaseOfCode;
	m_dwSizeOfCode = inh->OptionalHeader.SizeOfCode;

	return TRUE;
}

LPVOID CByteScanner::FindCodePattern( LPCVOID lpBytes, LPCSTR lpMask )
{
	if ( !m_dwBaseOfCode )
		return NULL;

	unsigned int unLength = strlen( lpMask );

	for ( unsigned int i = 0; i < m_dwSizeOfCode; i++ )
	{
		bool bMatch = true;

		for ( unsigned int j = 0; j < unLength; j++ )
		{
			if ( *(char *)(lpMask+j) == 'x' && *(char *)(m_dwBaseOfCode+i+j) != *(((char *)lpBytes)+j) )
			{
				bMatch = false;

				break;
			}
		}

		if ( bMatch )
			return (LPVOID)(m_dwBaseOfCode+i);
	}

	return NULL;
}