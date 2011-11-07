#include "dxrenderer.h"

CDXRenderer::CDXRenderer()
{
	m_D3D = NULL;
	m_D3DDevice = NULL;
}

bool CDXRenderer::Init( HWND hWnd, bool windowed, int width, int height )
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
	
	m_VerticesLocation = VERTICESLOCATION_SCREEN;

	return true;
}

void CDXRenderer::Shutdown()
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

void CDXRenderer::BeginFrame()
{
	m_D3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
	m_D3DDevice->BeginScene();	
}

void CDXRenderer::EndFrame()
{
	m_D3DDevice->EndScene();
	m_D3DDevice->Present( NULL, NULL, NULL, NULL );
}

void CDXRenderer::SetVerticesLocation( VERTICESLOCATION location )
{
	m_VerticesLocation = location;
}

void CDXRenderer::SetDrawColor( DWORD color )
{
	m_DrawColor = color;
}

void CDXRenderer::SetDrawColor( int r, int g, int b )
{
	m_DrawColor = D3DCOLOR_XRGB( r, g, b );
}

void CDXRenderer::SetTextColor( DWORD color )
{
	m_TextColor = color;
}

void CDXRenderer::SetTextColor( int r, int g, int b )
{
	m_TextColor = D3DCOLOR_XRGB( r, g, b );
}

void CDXRenderer::SetTextPos( float x, float y )
{
	m_TextPosX = x;
	m_TextPosY = y;
}

bool CDXRenderer::CreateFont( const char *typeface, const char *identifier, int size, int weight )
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

void CDXRenderer::SetFont( const char *identifier )
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

bool CDXRenderer::CreateTexture( const char *filename, const char *identifier )
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

void CDXRenderer::SetTexture( const char *identifier )
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

bool CDXRenderer::WorldTransformation( float x, float y, float z, float rx, float ry, float rz )
{
	D3DXMATRIX translate;
	D3DXMATRIX rotateX, rotateY, rotateZ;
	HRESULT err;

	// Translation
	D3DXMatrixTranslation( &translate, x, y, z );

	// Rotation
	D3DXMatrixRotationX( &rotateX, rx );
	D3DXMatrixRotationY( &rotateY, ry );
	D3DXMatrixRotationZ( &rotateZ, rz );

	err = m_D3DDevice->SetTransform( D3DTS_WORLD, &(rotateX * rotateY * rotateZ * translate) );

	if ( err != D3D_OK )
		return false;

	return true;
}

bool CDXRenderer::ViewTransformation()
{
	D3DXMATRIX view;
	HRESULT err;

	D3DXMatrixLookAtLH( &view,
						&D3DXVECTOR3( 0.0f, 0.0f, 300.0f ),
						&D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
						&D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );

	err = m_D3DDevice->SetTransform( D3DTS_VIEW, &view );

	if ( err != D3D_OK )
		return false;

	return true;
}

bool CDXRenderer::ProjectionTransformation( float aspectRatio )
{
	D3DXMATRIX projection;
	HRESULT err;

	D3DXMatrixPerspectiveFovLH( &projection,
								D3DXToRadian( 45 ),
								aspectRatio,
								1.0f,
								1000.0f );

	err = m_D3DDevice->SetTransform( D3DTS_PROJECTION, &projection );

	if ( err != D3D_OK )
		return false;

	return true;
}

bool CDXRenderer::DrawVertices( VERTICESDESCRIPTOR vertices, DWORD VertexSize, DWORD FVF )
{
	IDirect3DVertexBuffer9 *buffer;

	if ( m_D3DDevice->CreateVertexBuffer( vertices.FVF.DataSize, 0, FVF, D3DPOOL_MANAGED, &buffer, NULL ) != D3D_OK )
		return false;

	void *mem = NULL;

	buffer->Lock( 0, 0, &mem, 0 );
	memcpy( mem, vertices.FVF.Data, vertices.FVF.DataSize );
	buffer->Unlock();

	m_D3DDevice->SetFVF( FVF );
	m_D3DDevice->SetStreamSource( 0, buffer, 0, VertexSize );
	m_D3DDevice->DrawPrimitive( vertices.Primitive.Type, 0, vertices.Primitive.Count );

	buffer->Release();

	return true;
}

bool CDXRenderer::DrawRect( float x, float y, float width, float height )
{
	VERTICESDESCRIPTOR vertices;

	vertices.Primitive.Type = D3DPT_TRIANGLESTRIP;
	vertices.Primitive.Count = 2;

	switch ( m_VerticesLocation )
	{
	case VERTICESLOCATION_SCREEN:
		{
			FVFDATASCREEN data[] = {
				{ x, y, 0.5f, 1.0f, m_DrawColor, 0.0f, 0.0f }, // UV(0,0)
				{ x + width, y, 0.5f, 1.0f, m_DrawColor, 1.0f, 0.0f }, // UV(1,0)
				{ x, y + height, 0.5f, 1.0f, m_DrawColor, 0.0f, 1.0f }, // UV(0,1)
				{ x + width, y + height, 0.5f, 1.0f, m_DrawColor, 1.0f, 1.0f }, // UV(1,1)
			};

			vertices.FVF.Data = data;
			vertices.FVF.DataSize = sizeof( data );

			return DrawVertices( vertices, sizeof( FVFDATASCREEN ), FVFSCREEN );
		}
	case VERTICESLOCATION_WORLD:
		{
			FVFDATAWORLD data[] ={
				{ x, y, 0.0f, m_DrawColor },
				{ x + width, y, 0.0f, m_DrawColor },
				{ x, y + height, 0.0f, m_DrawColor },
				{ x + width, y + height, 0.0f, m_DrawColor },
			};

			vertices.FVF.Data = data;
			vertices.FVF.DataSize = sizeof( data );

			return DrawVertices( vertices, sizeof( FVFDATAWORLD ), FVFWORLD );
		}
	default:
		{
			return false;
		}
	}

	
}

bool CDXRenderer::DrawTexturedRect( float x, float y, float width, float height )
{
	m_D3DDevice->SetTexture( 0, m_D3DTexture );

	bool r = DrawRect( x, y, width, height );

	m_D3DDevice->SetTexture( 0, NULL );

	return r;
}

bool CDXRenderer::DrawLine( float xstart, float ystart, float xfinish, float yfinish )
{
	VERTICESDESCRIPTOR vertices;

	vertices.Primitive.Type = D3DPT_LINELIST;
	vertices.Primitive.Count = 1;

	switch ( m_VerticesLocation )
	{
	case VERTICESLOCATION_SCREEN:
		{
			FVFDATASCREEN data[] = {
				{ xstart, ystart, 0.5f, 1.0f, m_DrawColor },
				{ xfinish, yfinish, 0.5f, 1.0f, m_DrawColor },
			};

			vertices.FVF.Data = data;
			vertices.FVF.DataSize = sizeof( data );
			
			return DrawVertices( vertices, sizeof( FVFDATASCREEN ), FVFSCREEN );
		}
	default:
		{
			return false;
		}
	}
}

bool CDXRenderer::DrawText( const char *text )
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
						  m_TextColor );

	// Draw text with calculated rect
	m_D3DFont->DrawTextA( NULL,
						  text,
						  -1,
						  &rect,
						  DT_LEFT, // temp
						  m_TextColor );

	return true;
}