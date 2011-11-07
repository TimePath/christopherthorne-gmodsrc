#ifndef DXRENDERER_H
#define DXRENDERER_H

#include "d3d9.h"
#include "d3dx9.h"

#include <vector>

#undef CreateFont
#undef DrawText

const DWORD FVFSCREEN = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

struct FVFDATASCREEN
{
	FLOAT x, y, z, rhw;
	DWORD color;
	FLOAT tu, tv;
};

const DWORD FVFWORLD = D3DFVF_XYZ | D3DFVF_DIFFUSE;

struct FVFDATAWORLD
{
	FLOAT x, y, z;
	DWORD color;
};

enum VERTICESLOCATION
{
	VERTICESLOCATION_SCREEN,
	VERTICESLOCATION_WORLD,
};

struct FVFDESCRIPTOR
{
	LPVOID Data;
	DWORD DataSize;
};

struct PRIMITIVEDESCRIPTOR
{
	D3DPRIMITIVETYPE Type;
	UINT Count;
};

struct VERTICESDESCRIPTOR
{
	PRIMITIVEDESCRIPTOR Primitive;
	FVFDESCRIPTOR FVF;
};

struct NAMEDFONT
{
	char name[64];
	ID3DXFont *font;
};

struct NAMEDTEXTURE
{
	char name[64];
	IDirect3DTexture9 *texture;
};

class CDXRenderer
{
public:
	CDXRenderer();

	bool Init( HWND hWnd, bool windowed, int width, int height );
	void Shutdown();

	void BeginFrame();
	void EndFrame();

	void SetVerticesLocation( VERTICESLOCATION location );

	void SetDrawColor( DWORD color );
	void SetDrawColor( int r, int g, int b );

	void SetTextColor( DWORD color );
	void SetTextColor( int r, int g, int b );

	void SetTextPos( float x, float y );

	bool CreateFont( const char *typeface, const char *identifier, int size, int weight );
	void SetFont( const char *identifier );

	bool CreateTexture( const char *filename, const char *identifier );
	void SetTexture( const char *identifier );

	bool WorldTransformation( float x, float y, float z, float rx, float ry, float rz );
	bool ViewTransformation();
	bool ProjectionTransformation( float aspectRatio );

	bool DrawVertices( VERTICESDESCRIPTOR vertices, DWORD VertexSize, DWORD FVF );
	
	bool DrawRect( float x, float y, float width, float height );
	bool DrawTexturedRect( float x, float y, float width, float height );
	bool DrawLine( float xstart, float ystart, float xfinish, float yfinish );
	bool DrawText( const char *text );

public:
	IDirect3D9 *m_D3D;
	IDirect3DDevice9 *m_D3DDevice;
	ID3DXFont *m_D3DFont;
	IDirect3DTexture9 *m_D3DTexture;
	
	D3DPRESENT_PARAMETERS m_D3DPP;
	
	VERTICESLOCATION m_VerticesLocation;

	DWORD m_DrawColor;
	DWORD m_TextColor;
	
	float m_TextPosX;
	float m_TextPosY;

	std::vector<NAMEDFONT *> m_Fonts;
	std::vector<NAMEDTEXTURE *> m_Textures;
};

#endif // DXRENDERER_H