#include "testgame.h"

#include <math.h>

namespace TestGame
{

bool Initialize()
{
	if ( !Render()->CreateTexture( "textures/test.png", "test" ) )
		return false;

	return true;
}

void Shutdown()
{
}

void Frame()
{
	for ( int i = 0; i < 22; i++ )
	{
		for ( int j = 0; j < 16; j++ )
		{
			Render()->SetDrawColor( 255, 255, 255 );
			Render()->SetTexture( "test" );
			Render()->DrawTexturedRect( 32*(i+1) + sin( (float)GetTickCount()/500 )*10, 32*(j+1), 32, 32 );
		}
	}
}

}