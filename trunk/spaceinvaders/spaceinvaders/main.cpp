// Win32 API
#include "window_win32.h"
// DirectX
#include "dxrenderer.h"

// Invader configuration
#define INVADER_ROW_COUNT 5
#define INVADER_ROW_POPULATION 12

// Speed configuration
#define SHIPSPEED 5
#define BULLETSPEED 6
#define INVADERCOLORSPEED 1
#define INVADERSPEED 0.1f

// Score configuration
#define KILLPOINTS 100

// Use window/fullscreen
#define WINDOWED true

// Resolution
#define ScrW() 800
#define ScrH() 600

// Utils
#define ScrWFl() (float)ScrW()
#define ScrHFl() (float)ScrH()

// Invader information
struct invader_t
{
	float x, y, w, h;
	DWORD color;
	bool alive;
};

// DirectX
CDXRenderer render;

// Invaders
invader_t invaders[INVADER_ROW_COUNT][INVADER_ROW_POPULATION];

// Ship offset
float shipx = 0;

// Bullet offsets
float bulletx = 0;
float bullety = 0;

// States
bool firing = false;
bool gameover = false;

// Kill counter
int killcount = 0;

// Score
unsigned int scoreval;
char scoretext[128];

// Get score
int GetScore()
{
	return scoreval;
}

// Set score
void SetScore( int val )
{
	scoreval = val;

	_itoa_s( val, scoretext, sizeof( scoretext ), 10 );
}

// Add score
void AddScore( int val )
{
	SetScore( GetScore() + val );
}

// Start game
void LoadInvaders()
{
	SetScore( 0 );

	for ( int i = 0; i < INVADER_ROW_COUNT; i++ )
	{
		int y = 100 + (i*50);
		int w = (i*4);
		int h = (i*4);

		for ( int j = 0; j < INVADER_ROW_POPULATION; j++ )
		{
			int x = 110 + (j*50) - (w/2);

			invaders[i][j].x = (float)x;
			invaders[i][j].y = (float)y;
			invaders[i][j].w = (float)24 + w;
			invaders[i][j].h = (float)24 + h;
			invaders[i][j].color = D3DCOLOR_XRGB( rand()%256, rand()%256, rand()%256 );
			invaders[i][j].alive = true;
		}
	}
}

// Resources
void LoadResources()
{
	// Fonts
	render.CreateFont( "Arial", "ScoreFont", 22, 20 );
	// Textures
	render.CreateTexture( "test.png", "TestTexture" );
	render.CreateTexture( "test.png", "TestTexture2" );
}

// Check if the ship's bullet has hit an invader
void CheckShipBulletCollision()
{
	for ( int i = 0; i < INVADER_ROW_COUNT; i++ )
	{
		for ( int j = 0; j < INVADER_ROW_POPULATION; j++ )
		{
			invader_t *invader = &invaders[i][j];

			if ( !invader->alive )
				continue;

			if ( bullety >= invader->y && bullety <= invader->y + invader->h )
			{
				if ( bulletx >= invader->x && bulletx <= invader->x + invader->w )
				{
					firing = false;

					invader->alive = false;

					killcount++;

					AddScore( KILLPOINTS );
					
					if ( killcount >= INVADER_ROW_COUNT * INVADER_ROW_POPULATION )
						gameover = true;

					return;
				}
			}
		}
	}	
}

// Draw the ship
void RenderShip()
{
	if ( GetAsyncKeyState( VK_RIGHT ) )
		shipx += SHIPSPEED;

	if ( GetAsyncKeyState( VK_LEFT ) )
		shipx -= SHIPSPEED;

	render.SetDrawColor( 0, 255, 0 );
	render.DrawRect( ScrW() / 2 - 25 + shipx, ScrH() - 90, 50, 17 );
	render.DrawRect( ScrW() / 2 - 22 + shipx, ScrH() - 94, 44, 4 );
	render.DrawRect( ScrW() / 2 - 6 + shipx, ScrH() - 100, 12, 6 );
	render.DrawRect( ScrW() / 2 - 2 + shipx, ScrH() - 104, 4, 4 );
}

// Draw the ship's bullet (if it exists)
void RenderShipBullet()
{
	if ( !firing && GetAsyncKeyState( VK_SPACE ) )
	{
		bulletx = ScrW() / 2 + shipx;
		bullety = ScrH() - 110;

		firing = true;
	}

	if ( firing )
	{
		render.SetDrawColor( 255, 255, 255 );
		render.DrawLine( bulletx, bullety, bulletx, bullety + 10 );

		bullety -= BULLETSPEED;

		if ( bullety <= 0 )
			firing = false;
		else
			CheckShipBulletCollision();
	}
}

// Draw the space invaders
void RenderInvaders()
{
	render.SetTexture( "TestTexture2" );

	for ( int i = 0; i < INVADER_ROW_COUNT; i++ )
	{
		for ( int j = 0; j < INVADER_ROW_POPULATION; j++ )
		{
			invader_t *invader = &invaders[i][j];

			if ( !invader->alive )
				continue;

			render.SetDrawColor( invader->color );
			//render.DrawRect( invader->x, invader->y, invader->w, invader->h );
			render.DrawTexturedRect( invader->x, invader->y, invader->w, invader->h );

			invader->color += INVADERCOLORSPEED;
			invader->y += INVADERSPEED;

			if ( invader->y + invader->h >= ScrH() - ( WINDOWED ? 35 : 0 ) )
			{
				gameover = true;

				return;
			}
		}
	}
}

// Draw score counter
void RenderScore()
{
	render.SetTextColor( 255, 255, 255 );
	render.SetFont( "ScoreFont" );

	render.SetTextPos( 20, 20 );
	render.DrawText( "Score: " );

	render.SetTextPos( 80, 20 );
	render.DrawText( scoretext );
}

#define GRID_X 10
#define GRID_Y 10
#define GRID_SPACING 40

// Game title (temp)
void RenderTitle()
{
	render.SetTextColor( 255, 255, 255 );
	render.SetTextPos( ScrW()/2, 40 );
	render.SetFont( "ScoreFont" );
	render.DrawText( "aaaaaaaaa" );
}

// Draw grid using GRID_X*GRID_Y
void RenderGrid()
{
	for ( int i = 0; i <= GRID_X; i++ )
	{
		// x-axis
		float x_centre = ScrW() / 2;
		float x_offset = (((float)GRID_X / 2) - i) * GRID_SPACING;
		float x_pos = x_centre - x_offset;

		// y-axis
		float y_centre = ScrH() / 2;
		float y_offset = (GRID_Y * GRID_SPACING) / 2;
		float y_start = y_centre - y_offset;
		float y_end = y_centre + y_offset;

		// container
		render.SetDrawColor( 255, 255, 255 ); // white lines
		render.DrawLine( x_pos, y_start, x_pos, y_end );
	}

	for ( int i = 0; i <= GRID_Y; i++ )
	{
		// x-axis
		float x_centre = ScrW() / 2;
		float x_offset = (GRID_X * GRID_SPACING) / 2;
		float x_start = x_centre - x_offset;
		float x_end = x_centre + x_offset;

		// y-axis
		float y_centre = ScrH() / 2;
		float y_offset = (((float)GRID_Y / 2) - i) * GRID_SPACING;
		float y_pos = y_centre - y_offset;	

		// container
		render.SetDrawColor( 255, 255, 255 ); // white lines
		render.DrawLine( x_start, y_pos, x_end, y_pos );

		// filler
		if ( i < GRID_Y )
		{
			for ( int j = 0; j < GRID_X; j++ )
			{
				render.SetDrawColor( rand()%256, rand()%256, rand()%256 );

				if ( rand()%2 == 0 )
					render.DrawRect( x_start + 1 + (GRID_SPACING * j), y_pos + 1, GRID_SPACING - 1, GRID_SPACING - 1 );
			}
		}
	}

	/*float x_spacing = ScrW() / GRID_X;

	for ( int i = 0; i < ( GRID_X - 1 ); i++ )
	{
		float x_pos = x_spacing*(i+1);

		render.DrawLine( x_pos, 0, x_pos, ScrH() );
	}

	float y_spacing = ScrH() / GRID_Y;

	for ( int i = 0; i < ( GRID_Y - 1 ); i++ )
	{
		float y_pos = y_spacing*(i+1);

		render.DrawLine( 0, y_pos, ScrW(), y_pos );
	}*/
}

// Objects using screen coordinates
void RenderScreenObjects()
{
	render.SetVerticesLocation( VERTICESLOCATION_SCREEN );

	/*render.m_D3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	render.m_D3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	render.m_D3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	render.m_D3DDevice->SetTexture( 0, test_texture );*/

	/*render.SetDrawColor( 255, 255, 255 );
	render.SetTexture( "TestTexture" );
	render.DrawTexturedRect( 100, 100, 128, 128 );

	render.SetDrawColor( 255, 0, 0 );
	render.DrawRect( 200, 200, 50, 50 );*/

	/*render.SetDrawColor( 255, 255, 255 );
	render.SetTexture( "TestTexture2" );

	for ( int i = 0; i < 10; i++ )
	{
		for ( int j = 0; j < 10; j++ )
		{
			render.DrawTexturedRect( i*128, j*128, 128, 128 );
		}
	}*/

	RenderShip();
	RenderShipBullet();
	RenderInvaders();
	RenderScore();
	
	//RenderTitle();
	//RenderGrid();
}

// 3D test
void RenderOrbit()
{
	/*static float x_rotate = 0.0f;
	static float y_rotate = 0.0f;

	for ( int i = 0; i < 10; i++ )
	{
		render.SetDrawColor( 200+(i*2), 0, 0 );
		render.WorldTransformation( i*5, 90 + i*2, 0, x_rotate, y_rotate, 0 );
		render.DrawRect( 0, 0, 10, 10 );
	}

	for ( int i = 0; i < 10; i++ )
	{
		render.SetDrawColor( 0, 200+(i*2), 0 );
		render.WorldTransformation( -i*5, 90 + i*2, 0, x_rotate, y_rotate, 0 );
		render.DrawRect( 0, 0, 10, 10 );
	}

	x_rotate += 0.05f;
	y_rotate += 0.05f;*/
}

// Objects using world coordinates
void RenderWorldObjects()
{
	render.SetVerticesLocation( VERTICESLOCATION_WORLD );
	render.ViewTransformation();
	render.ProjectionTransformation( ScrW() / ScrH() );
	
	RenderOrbit();
}

// Render game frame
void RenderFrame()
{
	render.BeginFrame();

	RenderWorldObjects();
	RenderScreenObjects();
	
	render.EndFrame();
}

// Win32 callbacks
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	case WM_DESTROY:
		{
			PostQuitMessage( 0 );

			break;
		}
	default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
	}

	return 0;
}

// Win32 entry point
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	LoadInvaders();

	CGameWindow window( hInstance, WndProc, "Space Invaders", ScrW(), ScrH(), WINDOWED );

	if ( render.Init( window.GetHandle(), WINDOWED, ScrW(), ScrH() ) )
	{
		LoadResources();

		bool quit = false;

		while ( !quit && !gameover )
		{
			MSG msg;

			while ( PeekMessageA( &msg, NULL, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );

				if ( msg.message == WM_QUIT )
				{
					quit = true;

					break;
				}
			}

			RenderFrame();
		}
	}

	render.Shutdown();

	return 0;
}