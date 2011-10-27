#ifndef CDETOUR_H
#define CDETOUR_H

class CDetour
{
public:
	void DrawError_H( const char *, ... );

	static void (CDetour::* DrawError_T)( const char *, ... );
};

void (CDetour::* CDetour::DrawError_T)( const char *, ... ) = NULL;

#endif