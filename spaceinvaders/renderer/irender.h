#ifndef IRENDER_H
#define IRENDER_H

#include "d3d9.h"

#undef CreateFont
#undef DrawText

class IRender
{
public:
	// Initiates the renderer
	virtual bool Init( HWND hWnd, bool windowed, int width, int height ) = 0;
	// Shuts down the renderer
	virtual void Shutdown() = 0;
	// Begins a scene
	virtual void BeginFrame() = 0;
	// Finishes and presents a scene
	virtual void EndFrame() = 0;
	// Sets the color used by text/shapes
	virtual void SetDrawColor( unsigned int color ) = 0;
	virtual void SetDrawColor( int r, int g, int b ) = 0;
	// Creates a font
	virtual bool CreateFont( const char *typeface, const char *identifier, int size, int weight ) = 0;
	// Sets the font used by text
	virtual void SetFont( const char *identifier ) = 0;
	// Creates a texture
	virtual bool CreateTexture( const char *filename, const char *identifier ) = 0;
	// Sets the texture used by shapes
	virtual void SetTexture( const char *identifier ) = 0;
	// Draws raw vertex data
	virtual bool DrawVertices( void *data, int size, int count, D3DPRIMITIVETYPE primitive ) = 0;
	// Draws a rectangle
	virtual bool DrawRect( float x, float y, float width, float height ) = 0;
	// Draws a textured rectangle
	virtual bool DrawTexturedRect( float x, float y, float width, float height ) = 0;
	// Draws a line
	virtual bool DrawLine( float xstart, float ystart, float xfinish, float yfinish ) = 0;
	// Draws text
	virtual bool DrawText( const char *text, float x, float y ) = 0;
};

#endif // IRENDER_H