#ifndef CBYTESCANNER_H
#define CBYTESCANNER_H

#include <windows.h>

class CByteScanner
{
public:
	CByteScanner();
	CByteScanner( LPCVOID lpAddress );
	CByteScanner( LPCSTR lpModuleName );

	BOOL FindCodeSegment( LPCVOID lpAddress );
	LPVOID FindCodePattern( LPCVOID lpBytes, LPCSTR lpMask );

private:
	DWORD m_dwBaseOfCode; // Base address of code segment
	DWORD m_dwSizeOfCode; // Size (in bytes) of code segment
};

#endif // CBYTESCANNER_H