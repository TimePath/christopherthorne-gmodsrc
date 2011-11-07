#include "irender.h"

#include "d3dx9.h"

#include <vector>
#include <windows.h>

#undef CreateFont
#undef DrawText

const DWORD FVFSCREEN = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

struct FVFDATASCREEN
{
	FLOAT x, y, z, rhw;
	DWORD color;
	FLOAT tu, tv;
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

class CRender : public IRender
{
public:
	// Constructor
	CRender();
	// IRender
	bool Init( HWND hWnd, bool windowed, int width, int height );
	void Shutdown();
	void BeginFrame();
	void EndFrame();
	void SetDrawColor( unsigned int color );
	void SetDrawColor( int r, int g, int b );
	bool CreateFont( const char *typeface, const char *identifier, int size, int weight );
	void SetFont( const char *identifier );
	bool CreateTexture( const char *filename, const char *identifier );
	void SetTexture( const char *identifier );
	bool DrawVertices( void *data, int size, int count, D3DPRIMITIVETYPE primitive );
	bool DrawRect( float x, float y, float width, float height );
	bool DrawTexturedRect( float x, float y, float width, float height );
	bool DrawLine( float xstart, float ystart, float xfinish, float yfinish );
	bool DrawText( const char *text, float x, float y );
private:
	// DirectX interfaces
	IDirect3D9 *m_D3D;
	IDirect3DDevice9 *m_D3DDevice;
	ID3DXFont *m_D3DFont;
	IDirect3DTexture9 *m_D3DTexture;
	// DirectX info
	D3DPRESENT_PARAMETERS m_D3DPP;
	// Current draw color
	unsigned int m_DrawColor;
	// Current text position
	float m_TextPosX;
	float m_TextPosY;
	// Resources
	std::vector<NAMEDFONT *> m_Fonts;
	std::vector<NAMEDTEXTURE *> m_Textures;
};