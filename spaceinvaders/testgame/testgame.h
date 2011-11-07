#ifndef TESTGAME_H
#define TESTGAME_H

#include "../renderer/render_interface.h"

namespace TestGame
{

bool Initialize();
void Shutdown();
void Frame();

}

#endif // TESTGAME_H