#include "crender.h"

CRender::CRender()
{
	m_D3D = NULL;
	m_D3DDevice = NULL;
}

bool CRender::Init( HWND hWnd, bool windowed, int width, int height )
{
	HRESULT err;

	m_D3D = Direct3DCreate9( D3D_SDK_VERSION );

	if ( !m_D3D )
		return false;

	m_D3DPP.BackBufferWidth = windowed ? 0 : width;
	m_D3DPP.BackBufferHeight = windowed ? 0 : height;
	m_D3DPP.BackBufferFormat = windowed ? D3DFMT_UNKNOWN : D3DFMT_X8R8G8B8;
	m_D3DPP.BackBufferCount = 0;
	m_D3DPP.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_D3DPP.MultiSampleQuality = 0;
	m_D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_D3DPP.hDeviceWindow = hWnd;
	m_D3DPP.Windowed = (BOOL)windowed;
	m_D3DPP.EnableAutoDepthStencil = FALSE;
	m_D3DPP.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	m_D3DPP.Flags = 0;
	m_D3DPP.FullScreen_RefreshRateInHz = 0;
	m_D3DPP.PresentationInterval = 0;

	err = m_D3D->CreateDevice( D3DADAPTER_DEFAULT,
							   D3DDEVTYPE_HAL,
							   hWnd,
							   D3DCREATE_SOFTWARE_VERTEXPROCESSING,
							   &m_D3DPP,
							   &m_D3DDevice );

	if ( err != D3D_OK )
		return false;

	m_D3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_D3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	return true;
}

void CRender::Shutdown()
{
	for ( unsigned int i = 0; i < m_Fonts.size(); i++ )
	{
		m_Fonts[i]->font->Release();

		delete m_Fonts[i];
	}

	for ( unsigned int i = 0; i < m_Textures.size(); i++ )
	{
		m_Textures[i]->texture->Release();

		delete m_Textures[i];
	}

	if ( m_D3D )
		m_D3D->Release();

	if ( m_D3DDevice )
		m_D3DDevice->Release();
}

void CRender::BeginFrame()
{
	m_D3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
	m_D3DDevice->BeginScene();	
}

void CRender::EndFrame()
{
	m_D3DDevice->EndScene();
	m_D3DDevice->Present( NULL, NULL, NULL, NULL );
}

void CRender::SetDrawColor( unsigned int color )
{
	m_DrawColor = color;
}

void CRender::SetDrawColor( int r, int g, int b )
{
	m_DrawColor = D3DCOLOR_XRGB( r, g, b );
}

bool CRender::CreateFont( const char *typeface, const char *identifier, int size, int weight )
{
	HRESULT err;
	ID3DXFont *font;

	err = D3DXCreateFontA( m_D3DDevice,
							size,
							0,
							weight,
							0,
							FALSE,
							DEFAULT_CHARSET,
							OUT_DEFAULT_PRECIS,
							DEFAULT_QUALITY,
							DEFAULT_PITCH | FF_DONTCARE,
							typeface,
							&font );

	if ( err != D3D_OK )
		return false;

	NAMEDFONT *namedfont = new NAMEDFONT;

	strcpy_s( namedfont->name, sizeof( namedfont->name ), identifier );
	
	namedfont->font = font;

	m_Fonts.push_back( namedfont );

	return true;
}

void CRender::SetFont( const char *identifier )
{
	for ( unsigned int i = 0; i < m_Fonts.size(); i++ )
	{
		if ( strcmp( m_Fonts[i]->name, identifier ) == NULL )
		{
			m_D3DFont = m_Fonts[i]->font;

			break;
		}
	}
}

bool CRender::CreateTexture( const char *filename, const char *identifier )
{
	HRESULT err;
	IDirect3DTexture9 *texture;

	err = D3DXCreateTextureFromFileA( m_D3DDevice,
									  filename,
									  &texture );

	if ( err != D3D_OK )
		return false;

	NAMEDTEXTURE *namedtexture = new NAMEDTEXTURE;

	strcpy_s( namedtexture->name, sizeof( namedtexture->name ), identifier );
	
	namedtexture->texture = texture;

	m_Textures.push_back( namedtexture );

	return true;
}

void CRender::SetTexture( const char *identifier )
{
	for ( unsigned int i = 0; i < m_Textures.size(); i++ )
	{
		if ( strcmp( m_Textures[i]->name, identifier ) == NULL )
		{
			m_D3DTexture = m_Textures[i]->texture;

			break;
		}
	}
}

bool CRender::DrawVertices( void *data, int size, int count, D3DPRIMITIVETYPE primitive )
{
	IDirect3DVertexBuffer9 *buffer = NULL;

	if ( m_D3DDevice->CreateVertexBuffer( size, 0, FVFSCREEN, D3DPOOL_MANAGED, &buffer, NULL ) != D3D_OK )
		return false;

	void *mem = NULL;

	buffer->Lock( 0, 0, &mem, 0 );
	memcpy( mem, data, size );
	buffer->Unlock();

	m_D3DDevice->SetFVF( FVFSCREEN );
	m_D3DDevice->SetStreamSource( 0, buffer, 0, sizeof( FVFDATASCREEN ) );
	m_D3DDevice->DrawPrimitive( primitive, 0, count );

	buffer->Release();

	return true;
}

bool CRender::DrawRect( float x, float y, float width, float height )
{
	FVFDATASCREEN data[] = {
		{ x, y, 0.5f, 1.0f, m_DrawColor, 0.0f, 0.0f }, // UV(0,0)
		{ x + width, y, 0.5f, 1.0f, m_DrawColor, 1.0f, 0.0f }, // UV(1,0)
		{ x, y + height, 0.5f, 1.0f, m_DrawColor, 0.0f, 1.0f }, // UV(0,1)
		{ x + width, y + height, 0.5f, 1.0f, m_DrawColor, 1.0f, 1.0f }, // UV(1,1)
	};

	return DrawVertices( data, sizeof( data ), 2, D3DPT_TRIANGLESTRIP );	
}

bool CRender::DrawTexturedRect( float x, float y, float width, float height )
{
	m_D3DDevice->SetTexture( 0, m_D3DTexture );

	bool r = DrawRect( x, y, width, height );

	m_D3DDevice->SetTexture( 0, NULL );

	return r;
}

bool CRender::DrawLine( float xstart, float ystart, float xfinish, float yfinish )
{
	FVFDATASCREEN data[] = {
		{ xstart, ystart, 0.5f, 1.0f, m_DrawColor },
		{ xfinish, yfinish, 0.5f, 1.0f, m_DrawColor },
	};
	
	return DrawVertices( data, sizeof( data ), 1, D3DPT_LINELIST );
}

bool CRender::DrawText( const char *text, float x, float y )
{
	RECT rect;
	rect.left = (LONG)m_TextPosX;
	rect.top = (LONG)m_TextPosY;
	rect.right = 0;
	rect.bottom = 0;

	// Get text size
	m_D3DFont->DrawTextA( NULL,
						  text,
						  -1,
						  &rect,
						  DT_CALCRECT,
						  m_DrawColor );

	// Draw text with calculated rect
	m_D3DFont->DrawTextA( NULL,
						  text,
						  -1,
						  &rect,
						  DT_LEFT, // temp
						  m_DrawColor );

	return true;
}